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

namespace helmholtz
{
    Helmholtz::Helmholtz(const int frameSize, const int overlap, const cant::sample_f bias)
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


        m_inputBuffer   = std::vector<cant::sample_f>(m_frameSize, 0.);
        m_inputBuffer2  = std::vector<cant::sample_f>(m_frameSize, 0.);
        m_processBuffer = std::vector<cant::sample_f>(2 * m_frameSize, 0.);
    }


/*********************************************************************************/
/******************************** public *****************************************/
/*********************************************************************************/


    void Helmholtz::inSamples(const cant::sample_f *in, int size)
    {
        int mask = m_frameSize - 1;

        // call analysis function when it is time
        if(!(m_timeIndex && ((m_frameSize / m_overlap) - 1)))
        {
            analyzeFrame();
        }
        while(size--)
        {
            m_inputBuffer[m_timeIndex] = *in++;
            ++m_timeIndex;
            m_timeIndex &= mask;
        }
    }
    void Helmholtz::inOutSamples(const cant::sample_f *in, cant::sample_f *out, int size)
    {
        int mask = m_frameSize - 1;
        int outIndex = 0;

        // call analysis function when it is time
        if(!(m_timeIndex && ((m_frameSize / m_overlap) - 1)))
        {
            analyzeFrame();
        }

        while(size--)
        {
            m_inputBuffer[m_timeIndex] = *in++;
            out[outIndex++] = m_processBuffer[m_timeIndex++];
            m_timeIndex &= mask;
        }
    }

    void Helmholtz::setFrameSize(int frame)
    {
        if(!(
                (frame==128)
                ||(frame==256)
                ||(frame==512)
                ||(frame==1024)
                ||(frame==2048)
                ))
        {
            frame = c_defaultFrameSize;
        }
        m_frameSize = frame;

        m_inputBuffer.resize(m_frameSize);
        m_inputBuffer2.resize(m_frameSize);
        m_processBuffer.resize(2 * m_frameSize);

        m_timeIndex = 0;
    }

    int Helmholtz::getFrameSize() const
    {
        return m_frameSize;
    }


    void Helmholtz::setOverlap(int overlap)
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


    void Helmholtz::setBias(cant::sample_f bias)
    {
        if(bias > 1.) bias = 1.;
        if(bias < 0.) bias = 0.;
        m_biasFactor = bias;
    }


    void Helmholtz::setMinRMS(cant::sample_f rms)
    {
        if(rms > 1.) rms = 1.;
        if(rms < 0.) rms = 0.;
        m_minRMS = rms;
    }


    cant::sample_f Helmholtz::getPeriod() const
    {
        return m_periodLength;
    }


    cant::sample_f Helmholtz::getFidelity() const
    {
        return m_fidelity;
    }


/*********************************************************************************/
/***************************** private procedures ********************************/
/*********************************************************************************/


// main analysis function
    void Helmholtz::analyzeFrame()
    {
        int n, tindex = m_timeIndex;
        int mask = m_frameSize - 1;
        cant::sample_f norm = 1. / sqrt(cant::sample_f(2 * m_frameSize));

        // copy input to processing buffer
        for(n=0; n < m_frameSize; n++)
        {
            m_processBuffer[n] = m_inputBuffer[tindex++ & mask] * norm;
        }

        // copy for normalization function
        for(n=0; n < m_frameSize; n++)
        {
            m_inputBuffer2[n] = m_inputBuffer[tindex++ & mask];
        }

        // zeropadding
        std::fill(m_processBuffer.begin() + m_frameSize, m_processBuffer.end(), 0.);

        // call analysis procedures
        autoCorrelation();
        normalize();
        pickPeak();
        periodAndFidelity();
    }


    void Helmholtz::autoCorrelation()
    {
        int n;
        int fftsize = m_frameSize * 2;

        m_fftPerformer.performRealForward(m_processBuffer);

        // compute power spectrum
        m_processBuffer[0] *= m_processBuffer[0]; // DC
        m_processBuffer[m_frameSize] *= m_processBuffer[m_frameSize]; // Nyquist

        for(n=1; n < m_frameSize; n++)
        {
            m_processBuffer[n] = m_processBuffer[n] * m_processBuffer[n]
                                 + m_processBuffer[fftsize - n] * m_processBuffer[fftsize - n]; // imag coefficients appear reversed
            m_processBuffer[fftsize - n] = 0.;

        }

        m_fftPerformer.performRealInverse(m_processBuffer);
    }


    void Helmholtz::normalize()
    {
        int n;
        int seek = static_cast<int>(std::floor(m_frameSize * c_defaultSeekLengthRatio));
        cant::sample_f signal1, signal2;

        // minimum RMS implemented as minimum autocorrelation at index 0
        // effectively this means possible white noise addition
        cant::sample_f rms = m_minRMS / std::sqrt(1. / static_cast<cant::sample_f>(m_frameSize));
        cant::sample_f minrzero = rms * rms;
        cant::sample_f rzero = m_processBuffer[0];
        if(rzero < minrzero)
        {
            rzero = minrzero;
        }
        double normintegral = rzero * 2.;

        // normalize biased autocorrelation function
        m_processBuffer[0] = 1.;
        for(n=1; n<seek; n++)
        {
            signal1 = m_inputBuffer2[n - 1];
            signal2 = m_inputBuffer2[m_frameSize - n];
            normintegral -= static_cast<double>(signal1 * signal1 + signal2 * signal2);
            m_processBuffer[n] /= static_cast<cant::sample_f>(normintegral * 0.5);
        }

        // flush instable function tail
        std::fill(m_processBuffer.begin() + seek, m_processBuffer.begin() + m_frameSize, 0.);
    }


// select the peak which most probably represents period length
    void Helmholtz::pickPeak()
    {
        int n, peakIndex=0;
        int seek = static_cast<int>(std::floor(static_cast<cant::sample_f>(m_frameSize) * c_defaultSeekLengthRatio));
        cant::sample_f maxValue = 0.;
        cant::sample_f bias = m_biasFactor / static_cast<cant::sample_f>(m_frameSize);    // user-controlled bias
        cant::sample_f realPeak;

        // skip main lobe
        n = std::distance(
                m_processBuffer.begin(),
                std::find_if(
                        m_processBuffer.begin(),
                        m_processBuffer.end(),
                        [](const auto& e) { return e < 0.; }
                        )
                        );
        // find interpolated / biased maximum in specially normalized autocorrelation function
        // interpolation finds the 'real maximum'
        // biasing favours the first candidate
        for(; n<seek-1; n++)
        {
            if(
                    (m_processBuffer[n] > m_processBuffer[n - 1])
                    && (m_processBuffer[n] > m_processBuffer[n + 1])
            )
            {
                /* local peak */
                realPeak = interpolate3max(m_processBuffer.data(), n);

                if((realPeak * (1. - static_cast<cant::sample_f>(n * bias))) > maxValue)
                {
                    maxValue = realPeak;
                    peakIndex = n;
                }
            }
        }
        m_periodIndex = peakIndex;
    }


    void Helmholtz::periodAndFidelity()
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


    inline cant::sample_f Helmholtz::interpolate3max(cant::sample_f *buf, int peakIndex)
    {
        cant::sample_f realpeak;

        cant::sample_f a = buf[peakIndex - 1];
        cant::sample_f b = buf[peakIndex];
        cant::sample_f c = buf[peakIndex + 1];

        realpeak = b + 0.5 * (0.5 * ((c - a) * (c - a)))
                       / (2 * b - a - c);

        return realpeak;

    }


    inline cant::sample_f Helmholtz::interpolate3phase(cant::sample_f *buf, int peakIndex)
    {
        cant::sample_f fraction;

        cant::sample_f a = buf[peakIndex - 1];
        cant::sample_f b = buf[peakIndex];
        cant::sample_f c = buf[peakIndex + 1];

        fraction = (0.5 * (c - a)) / ( 2. * b - a - c);

        return fraction;
    }

}


