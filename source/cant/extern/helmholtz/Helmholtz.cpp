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
    : _fftPerformer(2 * frameSize),
    _timeIndex(0),
    _periodIndex(0),
    _minRMS(m_DEFMINRMS),
    _inputBuffer(),
    _inputBuffer2(),
    _processBuffer(),
    _periodLength(0.),
    _fidelity(0.)
    {
        setFrameSize(frameSize);
        setOverlap(overlap);
        setBias(bias);


        _inputBuffer   = std::vector<cant::sample_f>(_frameSize, 0.);
        _inputBuffer2  = std::vector<cant::sample_f>(_frameSize, 0.);
        _processBuffer = std::vector<cant::sample_f>(2 * _frameSize, 0.);
    }


/*********************************************************************************/
/******************************** public *****************************************/
/*********************************************************************************/


    void Helmholtz::inSamples(const cant::sample_f *in, int size)
    {
        int mask = _frameSize - 1;

        // call analysis function when it is time
        if(!(_timeIndex && ((_frameSize / _overlap) - 1)))
        {
            analyzeFrame();
        }
        while(size--)
        {
            _inputBuffer[_timeIndex] = *in++;
            ++_timeIndex;
            _timeIndex &= mask;
        }
    }
    void Helmholtz::inOutSamples(const cant::sample_f *in, cant::sample_f *out, int size)
    {
        int mask = _frameSize - 1;
        int outIndex = 0;

        // call analysis function when it is time
        if(!(_timeIndex && ((_frameSize / _overlap) - 1)))
        {
            analyzeFrame();
        }

        while(size--)
        {
            _inputBuffer[_timeIndex] = *in++;
            out[outIndex++] = _processBuffer[_timeIndex++];
            _timeIndex &= mask;
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
            frame = m_DEFFRAMESIZE;
        }
        _frameSize = frame;

        _inputBuffer.resize(_frameSize);
        _inputBuffer2.resize(_frameSize);
        _processBuffer.resize(2 * _frameSize);

        _timeIndex = 0;
    }

    int Helmholtz::getFrameSize() const
    {
        return _frameSize;
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
            overlap = m_DEFOVERLAP;
        }
        _overlap = overlap;
    }


    void Helmholtz::setBias(cant::sample_f bias)
    {
        if(bias > 1.) bias = 1.;
        if(bias < 0.) bias = 0.;
        _biasFactor = bias;
    }


    void Helmholtz::setMinRMS(cant::sample_f rms)
    {
        if(rms > 1.) rms = 1.;
        if(rms < 0.) rms = 0.;
        _minRMS = rms;
    }


    cant::sample_f Helmholtz::getPeriod() const
    {
        return _periodLength;
    }


    cant::sample_f Helmholtz::getFidelity() const
    {
        return _fidelity;
    }


/*********************************************************************************/
/***************************** private procedures ********************************/
/*********************************************************************************/


// main analysis function
    void Helmholtz::analyzeFrame()
    {
        int n, tindex = _timeIndex;
        int mask = _frameSize - 1;
        cant::sample_f norm = 1. / sqrt(cant::sample_f(2 * _frameSize));

        // copy input to processing buffer
        for(n=0; n<_frameSize; n++)
        {
            _processBuffer[n] = _inputBuffer[tindex++ & mask] * norm;
        }

        // copy for normalization function
        for(n=0; n<_frameSize; n++)
        {
            _inputBuffer2[n] = _inputBuffer[tindex++ & mask];
        }

        // zeropadding
        std::fill(_processBuffer.begin() + _frameSize, _processBuffer.end(), 0.);

        // call analysis procedures
        autoCorrelation();
        normalize();
        pickPeak();
        periodAndFidelity();
    }


    void Helmholtz::autoCorrelation()
    {
        int n;
        int fftsize = _frameSize * 2;

        _fftPerformer.performRealForward(_processBuffer);

        // compute power spectrum
        _processBuffer[0] *= _processBuffer[0]; // DC
        _processBuffer[_frameSize] *= _processBuffer[_frameSize]; // Nyquist

        for(n=1; n<_frameSize; n++)
        {
            _processBuffer[n] = _processBuffer[n] * _processBuffer[n]
                            + _processBuffer[fftsize-n] * _processBuffer[fftsize-n]; // imag coefficients appear reversed
            _processBuffer[fftsize-n] = 0.;

        }

        _fftPerformer.performRealInverse(_processBuffer);
    }


    void Helmholtz::normalize()
    {
        int n;
        int seek = static_cast<int>(std::floor(_frameSize * m_SEEK));
        cant::sample_f signal1, signal2;

        // minimum RMS implemented as minimum autocorrelation at index 0
        // effectively this means possible white noise addition
        cant::sample_f rms = _minRMS / sqrt(1. / (cant::sample_f)_frameSize);
        cant::sample_f minrzero = rms * rms;
        cant::sample_f rzero = _processBuffer[0];
        if(rzero < minrzero)
        {
            rzero = minrzero;
        }
        double normintegral = rzero * 2.;

        // normalize biased autocorrelation function
        _processBuffer[0] = 1.;
        for(n=1; n<seek; n++)
        {
            signal1 = _inputBuffer2[n-1];
            signal2 = _inputBuffer2[_frameSize-n];
            normintegral -= (double)(signal1 * signal1 + signal2 * signal2);
            _processBuffer[n] /= (cant::sample_f)normintegral * 0.5;
        }

        // flush instable function tail
        std::fill(_processBuffer.begin() + seek, _processBuffer.begin() + _frameSize, 0.);
    }


// select the peak which most probably represents period length
    void Helmholtz::pickPeak()
    {
        int n, peakIndex=0;
        int seek = static_cast<int>(std::floor(static_cast<cant::sample_f>(_frameSize) * m_SEEK));
        cant::sample_f maxValue = 0.;
        cant::sample_f bias = _biasFactor / static_cast<cant::sample_f>(_frameSize);    // user-controlled bias
        cant::sample_f realPeak;

        // skip main lobe
        n = std::distance(
                _processBuffer.begin(),
                std::find_if(
                        _processBuffer.begin(),
                        _processBuffer.end(),
                        [](const auto& e) { return e < 0.; }
                        )
                        );
        // find interpolated / biased maximum in specially normalized autocorrelation function
        // interpolation finds the 'real maximum'
        // biasing favours the first candidate
        for(; n<seek-1; n++)
        {
            if(
                    (_processBuffer[n] > _processBuffer[n-1])
                    && (_processBuffer[n] > _processBuffer[n+1])
            )
            {
                /* local peak */
                realPeak = interpolate3max(_processBuffer.data(), n);

                if((realPeak * (1. - (cant::sample_f)n * bias)) > maxValue)
                {
                    maxValue = realPeak;
                    peakIndex = n;
                }
            }
        }
        _periodIndex = peakIndex;
    }


    void Helmholtz::periodAndFidelity()
    {
        if(_periodIndex)
        {
            _periodLength = static_cast<cant::sample_f>(_periodIndex) + interpolate3phase(_processBuffer.data(), _periodIndex);
            _fidelity = interpolate3max(_processBuffer.data(), _periodIndex);
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


