/*
Class Helmholtz implements a period-length detector using Philip McLeod's
Specially Normalized AutoCorrelation function (SNAC).

Licensed under three-clause BSD license.

Katja Vetter, Feb 2012.

*/

#include <cant/extern/helmholtz/Helmholtz.hpp>

#include <iostream>
#include <cmath>
#include <algorithm>

#include <cant/common/macro.hpp>
namespace helmholtz
{
    Helmholtz::
    Helmholtz(cant::size_u frameSize, cant::type_i overlap, cant::type_d bias)
    : m_fftPerformer(2 * frameSize),
      m_timeIndex(0),
      m_periodIndex(0),
      m_minRMS(c_defaultMinRMS),
      m_inputBuffer(),
      m_inputBuffer2(),
      m_processBuffer(),
      m_periodLength(0.),
      m_fidelity(0.)
    {
        setFrameSize(frameSize);
        setOverlap(overlap);
        setBias(bias);


        m_inputBuffer   = std::vector<cant::sample_f>(static_cast<cant::size_u>(m_frameSize), 0.);
        m_inputBuffer2  = std::vector<cant::sample_f>(static_cast<cant::size_u>(m_frameSize), 0.);
        m_processBuffer = std::vector<cant::sample_f>(2 * static_cast<cant::size_u>(m_frameSize), 0.);
    }


/*********************************************************************************/
/******************************** public *****************************************/
/*********************************************************************************/


    void
    Helmholtz::
    inSamples(const cant::sample_f *in, int size)
    {
        int mask = static_cast<int>(m_frameSize) - 1;

        // call analysis function when it is time
        if(!(m_timeIndex && ((static_cast<int>(m_frameSize) / m_overlap) - 1)))
        {
            analyzeFrame();
        }
        while(size--)
        {
            m_inputBuffer[m_timeIndex] = *in++;
            ++m_timeIndex;
            m_timeIndex &= static_cast<cant::size_u>(mask);
        }
    }
    void
    Helmholtz::
    inOutSamples(const cant::sample_f *in, cant::sample_f *out, int size)
    {
        int mask = static_cast<int>(m_frameSize) - 1;
        int outIndex = 0;

        // call analysis function when it is time
        if(!(m_timeIndex && ((static_cast<int>(m_frameSize) / m_overlap) - 1)))
        {
            analyzeFrame();
        }

        while(size--)
        {
            m_inputBuffer[m_timeIndex] = *in++;
            out[outIndex++] = m_processBuffer[m_timeIndex++];
            m_timeIndex &= static_cast<cant::size_u>(mask);
        }
    }

    void
    Helmholtz::
    setFrameSize(cant::size_u frameSize)
    {
        if(!(
                (frameSize==128)
                ||(frameSize==256)
                ||(frameSize==512)
                ||(frameSize==1024)
                ||(frameSize==2048)
                ))
        {
            frameSize = c_defaultFrameSize;
        }
        m_frameSize = frameSize;

        m_inputBuffer.resize(m_frameSize);
        m_inputBuffer2.resize(m_frameSize);
        m_processBuffer.resize(2 * m_frameSize);

        m_timeIndex = 0;
    }

    CANT_NODISCARD
    cant::size_u
    Helmholtz::
    getFrameSize() const
    {
        return m_frameSize;
    }


    void
    Helmholtz::
    setOverlap(int overlap)
    {
        if(!(
                (overlap == 1)
                || (overlap == 2)
                || (overlap == 4)
                || (overlap == 8)
                ))
        {
            overlap = c_defaultOverlap;
        }
        m_overlap = overlap;
    }


    void
    Helmholtz::
    setBias(cant::type_d bias)
    {
        if(bias > 1.) bias = 1.;
        if(bias < 0.) bias = 0.;
        m_biasFactor = bias;
    }


    void
    Helmholtz::
    setMinRMS(cant::type_d rms)
    {
        if(rms > 1.) rms = 1.;
        if(rms < 0.) rms = 0.;
        m_minRMS = rms;
    }


    CANT_NODISCARD
    cant::type_d
    Helmholtz::
    getPeriod() const
    {
        return static_cast<cant::sample_f>(m_periodLength);
    }

    CANT_NODISCARD
    cant::type_d
    Helmholtz::
    getFidelity() const
    {
        return static_cast<cant::sample_f>(m_fidelity);
    }


/*********************************************************************************/
/***************************** private procedures ********************************/
/*********************************************************************************/


// main analysis function
    void
    Helmholtz::
    analyzeFrame()
    {
        cant::size_u n;
        int tindex = static_cast<int>(m_timeIndex);
        int mask = static_cast<int>(m_frameSize) - 1;
        const auto norm = static_cast<cant::sample_f>(1. / sqrt(cant::sample_f(2 * m_frameSize)));

        // copy input to processing buffer
        for(n=0; n < m_frameSize; n++)
        {
            m_processBuffer.at(n) = m_inputBuffer.at(static_cast<cant::size_u>(tindex++ & mask)) * norm;
        }

        // copy for normalization function
        for(n=0; n < m_frameSize; n++)
        {
            m_inputBuffer2.at(n) = m_inputBuffer.at(static_cast<cant::size_u>(tindex++ & mask));
        }
        // zeropadding
        std::fill(
                m_processBuffer.begin() + static_cast<long int>(m_frameSize),
                m_processBuffer.end(),
                static_cast<cant::sample_f>(0.)
                );

        // call analysis procedures
        autoCorrelation();
        normalize();
        pickPeak();
        periodAndFidelity();
    }

    void
    Helmholtz::
    autoCorrelation()
    {
        cant::size_u n;
        int fftsize = static_cast<int>(m_frameSize) * 2;

        m_fftPerformer.performRealForward(m_processBuffer);

        // compute power spectrum
        m_processBuffer.at(0) *= m_processBuffer.at(0); // DC
        m_processBuffer.at(m_frameSize) *= m_processBuffer.at(m_frameSize); // Nyquist

        for(n=1; n < m_frameSize; n++)
        {
            const auto convIndex = static_cast<cant::size_u>(fftsize - static_cast<int>(n));
            m_processBuffer[n] = m_processBuffer.at(n) * m_processBuffer.at(n)
                                 + m_processBuffer.at(convIndex)
                                    * m_processBuffer.at(convIndex); // imag coefficients appear reversed
            m_processBuffer.at(convIndex) = static_cast<cant::sample_f>(0.);

        }

        m_fftPerformer.performRealInverse(m_processBuffer);
    }

    void
    Helmholtz::
    normalize()
    {
        const auto seek = static_cast<cant::size_u>(
                std::floor(static_cast<cant::type_d>(m_frameSize) * c_defaultSeekLengthRatio)
                );

        cant::type_d normIntegral;
        {
            // minimum RMS implemented as minimum autocorrelation at index 0
            // effectively this means possible white noise addition
            const auto rms =
                    static_cast<cant::sample_f>(
                            m_minRMS / std::sqrt(1. / static_cast<cant::sample_f>(m_frameSize))
                    );
            const cant::sample_f minRZero = rms * rms;
            cant::sample_f rZero = m_processBuffer.at(0);
            if(rZero < minRZero)
            {
                rZero = minRZero;
            }
            normIntegral = rZero * 2.;
        }

        // normalize biased autocorrelation function
        m_processBuffer[0] = 1.;
        for(cant::size_u n = 1; n < seek; n++)
        {
            cant::sample_f signal1 = m_inputBuffer2.at(static_cast<cant::size_u>(n - 1));
            cant::sample_f signal2 = m_inputBuffer2.at(m_frameSize - n);
            normIntegral -= static_cast<double>(signal1 * signal1 + signal2 * signal2);
            m_processBuffer.at(n)  /= static_cast<cant::sample_f>(normIntegral * 0.5);
        }

        // flush unstable function tail
        std::fill(
                m_processBuffer.begin() + static_cast<unsigned int>(seek),
                m_processBuffer.begin() + static_cast<unsigned int>(m_frameSize),
                static_cast<cant::sample_f>(0.)
                );
    }

// select the peak which most probably represents period length
    void
    Helmholtz::
    pickPeak()
    {
        const auto seek = static_cast<cant::size_u>(std::floor(static_cast<cant::sample_f>(m_frameSize) * c_defaultSeekLengthRatio));
        const auto bias =
                static_cast<cant::sample_f>(
                        m_biasFactor / static_cast<cant::sample_f>(m_frameSize)
                        );    // user-controlled bias

        // skip main lobe
        auto n = static_cast<cant::size_u>(
                std::round(
                std::distance(
                m_processBuffer.begin(),
                std::find_if(
                        m_processBuffer.begin(),
                        m_processBuffer.end(),
                        [](const auto& e) { return e < 0.; }
                        )
                )
        ));
        // find interpolated / biased maximum in specially normalized autocorrelation function
        // interpolation finds the 'real maximum'
        // biasing favours the first candidate
        cant::sample_f maxValue = 0.;
        cant::sample_f realPeak;
        cant::size_u peakIndex=0;
        for(; n < seek-1; n++)
        {
            if(
                    (m_processBuffer.at(n) > m_processBuffer.at(n - 1))
                    && (m_processBuffer.at(n) > m_processBuffer.at(n + 1))
            )
            {
                /* local peak */
                realPeak = interpolate3max(m_processBuffer.data(), n);

                if((realPeak * (1. - static_cast<cant::sample_f>(static_cast<cant::type_d>(n) * bias))) > maxValue)
                {
                    maxValue = realPeak;
                    peakIndex = n;
                }
            }
        }
        m_periodIndex = peakIndex;
    }

    void
    Helmholtz::
    periodAndFidelity()
    {
        if(m_periodIndex)
        {
            m_periodLength = static_cast<cant::sample_f>(m_periodIndex) + interpolate3phase(m_processBuffer.data(), m_periodIndex);
            m_fidelity = interpolate3max(m_processBuffer.data(), m_periodIndex);
        }
    }


/*********************************************************************************/
/***************************** private functions *********************************/
/*********************************************************************************/


    CANT_NODISCARD
    cant::sample_f
    Helmholtz::
    interpolate3max(const cant::sample_f *buf, cant::size_u peakIndex)
    {
        cant::sample_f realpeak;

        cant::sample_f a = buf[peakIndex - 1];
        cant::sample_f b = buf[peakIndex];
        cant::sample_f c = buf[peakIndex + 1];

        realpeak = static_cast<cant::sample_f>(
                b + 0.5 * (0.5 * ((c - a) * (c - a)))
                       / (2 * b - a - c)
        );
        return realpeak;
    }

    CANT_NODISCARD
    cant::sample_f
    Helmholtz::
    interpolate3phase(const cant::sample_f *buf, cant::size_u peakIndex)
    {
        cant::sample_f fraction;

        cant::sample_f a = buf[peakIndex - 1];
        cant::sample_f b = buf[peakIndex];
        cant::sample_f c = buf[peakIndex + 1];

        fraction = static_cast<cant::sample_f>((0.5 * (c - a)) / ( 2. * b - a - c));

        return fraction;
    }
}


