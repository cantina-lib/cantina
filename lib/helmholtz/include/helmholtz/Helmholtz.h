#ifndef Helmholtz_H
#define Helmholtz_H

#pragma once

/***********************************************************************

Class Helmholtz implements a period-length detector using Philip McLeod's
Specially Normalized AutoCorrelation function (SNAC).

Function iosamples() takes a pointer to a buffer with n signal input samples 
and a pointer to a buffer where n output samples are stored, 
representing the SNAC function.

Via function setframesize(), analysis frame size can be set to 
128, 256, 512, 1024 or 2048 samples. Default is 1024.

With setoverlap(), analysis frames can be set to overlap each other 
with factor 1, 2, 4 or 8. Default is 1.

Function setbias() sets a bias which favours small lags over large lags in 
the period detection, thereby avoiding low-octave jumps. Default is 0.2

Function setminRMS() is used as a sensitivity setting. Default is RMS 0.003.

With function getperiod(), the last detected period length is returned 
as number of samples with a possible fraction (floating point format). 

Function getfidelity() returns a value between 0. and 1. indicating
to which extent the input signal is periodic. A fidelity of ~0.95 can
be considered to indicate a periodic signal.

Class Helmholtz needs mayer_realfft() and mayer_realifft() or similar
fft functions. Note that Ron Mayer's functions for real fft have a
peculiar organisation of imaginary coefficients (reversed order, sign flipped).

Class Helmholtz uses cant::sample_m for float or double. Depending on the context
where the class is used, you may need to define cant::sample_m. If used with
PD or MaxMsp, it is already defined.

***********************************************************************

Licensed under three-clause BSD license.
 
Katja Vetter, Feb 2012.

***********************************************************************/

/* This section includes the Pure Data API header. If you build Helmholtz
against another DSP framework, you need to define t_float, and you need to
include Ron Mayer's fft or similar functionality. */
// pip: Using internal sample_m (float), and fftw3 for fourier's discrete transform

#include <cant/common/types.hpp>
#include <cant/fft/FFTWPerformer.hpp>

#include <vector>

/***********************************************************************/
namespace helmholtz
{
    static constexpr int DEFFRAMESIZE = 1024;      // default analysis framesize
    static constexpr int DEFOVERLAP = 1;            // default overlap
    static constexpr double DEFBIAS = 0.2;             // default bias
    static constexpr double DEFMINRMS = 0.003;        // default minimum RMS
    static constexpr double SEEK = 0.85;              // seek-length as ratio of framesize


    class Helmholtz
    {
    public:
        Helmholtz(int periodarg = DEFFRAMESIZE, int overlaparg = DEFOVERLAP, cant::sample_m biasarg = DEFBIAS);

        ~Helmholtz() = default;
        void inOutSamples(const cant::sample_m *in, cant::sample_m *out, int size);
        void inSamples(const cant::sample_m *in, int size);
        void setFrameSize(int frame);
        void setOverlap(int lap);
        void setBias(cant::sample_m bias);
        void setMinRMS(cant::sample_m rms);
        cant::sample_m getPeriod() const;
        cant::sample_m getFidelity() const;
        int getFrameSize() const;

    private:
        cant::fft::FFTWPerformer _fftPerformer;
        // procedures
        void analyzeFrame();
        void autoCorrelation();
        void normalize();
        void pickPeak();
        void periodAndFidelity();

        // functions
        cant::sample_m interpolate3max(cant::sample_m *buf, int peakIndex);
        cant::sample_m interpolate3phase(cant::sample_m *buf, int peakIndex);

        // state variables
        int _frameSize;
        int _timeIndex;
        int _overlap;
        int _periodIndex;

        cant::sample_m _biasFactor;
        cant::sample_m _minRMS;

        // buffers
        std::vector<cant::sample_m> _inputBuffer;
        std::vector<cant::sample_m> _inputBuffer2;
        std::vector<cant::sample_m> _processBuffer;


        // output
        cant::sample_m _periodLength;
        cant::sample_m _fidelity;
    };

}

#endif // #ifndef Helmholtz_H