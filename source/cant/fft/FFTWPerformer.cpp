//
// Created by binabik on 26/07/2020.
//

#include <cant/fft/FFTWPerformer.hpp>

#include <cant/common/CantinaException.hpp>

namespace cant::fft
{

    FFTWPerformer::
    FFTWPerformer(const sizeint blockSize)
    : _blockSize(blockSize)
    {
        _inoutBuffer = (sample_m*) fftw_malloc(sizeof(sample_m) * 2 * blockSize);
        if (!_inoutBuffer)
        {
            throw CANTINA_EXCEPTION("Could not initialise buffer.");
        }

        _forwardPlan = fftwf_plan_r2r_1d(blockSize, _inoutBuffer, _inoutBuffer, FFTW_R2HC, FFTW_PATIENT);
        _inversePlan = fftwf_plan_r2r_1d(blockSize, _inoutBuffer, _inoutBuffer, FFTW_HC2R, FFTW_PATIENT);
    }

    FFTWPerformer::
    ~FFTWPerformer()
    {
        fftwf_destroy_plan(_forwardPlan);
        fftwf_destroy_plan(_inversePlan);
        fftw_free(_inoutBuffer);
    }

    void
    FFTWPerformer::
    perform(sample_m *inout, sizeint blockSize, const fftwf_plan& plan)
    {
        /*
         * the received sample has not been allocated with fftw_malloc,
         * so for alignment reasons, we should process from
         * the internal _inoutBuffer
         * then copy the result.
         */
        CANTINA_ASSERT(_blockSize == blockSize);
        memcpy(_inoutBuffer, inout, blockSize);
        fftwf_execute(plan);
        memcpy(_inoutBuffer, inout, blockSize);
    }

    void
    FFTWPerformer::
    performForward(sample_m* inout, const sizeint blockSize)
    {
        perform(inout, blockSize, _forwardPlan);
    }

    void
    FFTWPerformer::
    performInverse(sample_m *inout, const sizeint blockSize)
    {
        perform(inout, blockSize, _inversePlan);
    }
}
