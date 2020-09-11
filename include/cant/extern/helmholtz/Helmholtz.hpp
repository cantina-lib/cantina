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
#include <cant/common/macro.hpp>
namespace helmholtz
{
    class Helmholtz
    {

    public:
        /** -- methods  -- **/
        CANT_EXPLICIT Helmholtz(
                int period,
                int overlap = c_defaultOverlap,
                cant::sample_f bias = c_defaultBias
                );

        void inOutSamples(const cant::sample_f *in, cant::sample_f *out, int size);
        void inSamples(const cant::sample_f *in, int size);
        void setFrameSize(int frame);
        void setOverlap(int lap);
        void setBias(cant::sample_f bias);
        void setMinRMS(cant::sample_f rms);
        CANT_NODISCARD cant::sample_f getPeriod() const;
        CANT_NODISCARD cant::sample_f getFidelity() const;
        CANT_NODISCARD int getFrameSize() const;

    private:
        /** -- methods  -- **/
        // procedures
        void analyzeFrame();
        void autoCorrelation();
        void normalize();
        void pickPeak();
        void periodAndFidelity();

        // functions
        cant::sample_f interpolate3max(const cant::sample_f *buf, int peakIndex);
        cant::sample_f interpolate3phase(const cant::sample_f *buf, int peakIndex);

        /** -- fields -- **/
        cant::fft::FFTWPerformer m_fftPerformer;

        // state variables
        int m_frameSize;
        int m_timeIndex;
        int m_overlap;
        int m_periodIndex;

        double m_biasFactor;
        double m_minRMS;

        // buffers
        std::vector<cant::sample_f> m_inputBuffer;
        std::vector<cant::sample_f> m_inputBuffer2;
        std::vector<cant::sample_f> m_processBuffer;


        // output
        cant::type_d m_periodLength;
        cant::type_d m_fidelity;

        // constants
        static CANT_CONSTEXPR int c_defaultFrameSize          = 1024;      // default analysis framesize
        static CANT_CONSTEXPR int c_defaultOverlap            = 1;            // default overlap
        static CANT_CONSTEXPR double c_defaultBias            = 0.2;             // default bias
        static CANT_CONSTEXPR double c_defaultMinRMS          = 0.003;        // default minimum RMS
        static CANT_CONSTEXPR double c_defaultSeekLengthRatio = 0.85;              // seek-length as ratio of framesize
    };

}

#include <cant/common/undef_macro.hpp>
#endif // #ifndef Helmholtz_H