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
CANTINA_CANT_NAMESPACE_BEGIN

    class Helmholtz
    {

    public:
        /** -- methods  -- **/
        CANT_EXPLICIT Helmholtz(
                size_u frameSize,
                type_i overlap = c_defaultOverlap,
                type_d bias = c_defaultBias
                );

        void inOutSamples(const sample_f *in, sample_f *out, size_u blockSize);
        void inSamples(const sample_f *in, size_u blockSize);
        void setFrameSize(size_u frameSize);
        void setOverlap(type_i overlap);
        void setBias(type_d bias);
        void setMinRMS(type_d rms);
        CANT_NODISCARD type_d getPeriod() const;
        CANT_NODISCARD type_d getFidelity() const;
        CANT_NODISCARD size_u getFrameSize() const;

    private:
        /** -- methods  -- **/
        // procedures
        void analyzeFrame();
        void autoCorrelation();
        void normalize();
        void pickPeak();
        void periodAndFidelity();

        // functions
        CANT_NODISCARD static sample_f interpolate3max  (const sample_f *buf, size_u peakIndex);
        CANT_NODISCARD static sample_f interpolate3phase(const sample_f *buf, size_u peakIndex);

        /** -- fields -- **/
        FFTWPerformer m_fftPerformer;

        // state variables
        size_u m_frameSize;
        size_u m_timeIndex;
        type_i m_overlap;
        size_u m_periodIndex;

        type_d m_biasFactor;
        type_d m_minRMS;

        // buffers
        Stream<cant::sample_f> m_inputBuffer;
        Stream<cant::sample_f> m_inputBuffer2;
        Stream<cant::sample_f> m_processBuffer;


        // output
        type_d m_periodLength;
        type_d m_fidelity;

        // constants
        static CANT_CONSTEXPR type_i c_defaultFrameSize          = 1024;      // default analysis framesize
        static CANT_CONSTEXPR type_i c_defaultOverlap            = 1;            // default overlap
        static CANT_CONSTEXPR type_d c_defaultBias            = 0.2;             // default bias
        static CANT_CONSTEXPR type_d c_defaultMinRMS          = 0.003;        // default minimum RMS
        static CANT_CONSTEXPR type_d c_defaultSeekLengthRatio = 0.85;              // seek-length as ratio of framesize
    };

CANTINA_CANT_NAMESPACE_END
#include <cant/common/undef_macro.hpp>

#endif // #ifndef Helmholtz_H