//
// Created by binabik on 26/07/2020.
//

#include <cant/fft/FFTWPerformer.hpp>

#include <cant/common/CantinaException.hpp>

#include <algorithm>

#include <cant/common/macro.hpp>
namespace cant::fft
{

    FFTWPerformer::
    FFTWPerformer(const size_m transformSize)
    : _inoutBuffer()
    {
        const sample_m* inout = fftwf_alloc_real(transformSize);
        if (!inout)
        {
            throw CANTINA_EXCEPTION("Could not initialise buffer.");
        }
        _inoutBuffer.assign(inout, inout + transformSize);

        _realForwardPlan = computeRealPlan(_inoutBuffer, FFTW_R2HC);
        _realInversePlan = computeRealPlan(_inoutBuffer, FFTW_HC2R);
    }

    fftwf_plan
    FFTWPerformer::
    computeRealPlan(Stream<sample_m>& inoutBuffer, const fftw_r2r_kind kind)
    {
        return fftwf_plan_r2r_1d(inoutBuffer.size(), inoutBuffer.data(), inoutBuffer.data(), kind, FFTW_PATIENT);
    }

    FFTWPerformer::
    ~FFTWPerformer()
    {
        fftwf_destroy_plan(_realForwardPlan);
        fftwf_destroy_plan(_realInversePlan);
        fftwf_free(_inoutBuffer.data());
    }

    void
    FFTWPerformer::
    performReal(Stream<sample_m>& inout, const fftwf_plan& realPlan)
    {
        /*
         * the received sample has not been allocated with fftwf_malloc,
         * so for alignment reasons, we should process from
         * the internal _inoutBuffer
         * then copy the result.
         */
        CANTINA_ASSERT(inout.size() == getTransformSize(), "Nope nope nope.");
        std::copy(inout.begin(), inout.end(), _inoutBuffer.begin());
        fftwf_execute(realPlan);
        std::copy(_inoutBuffer.begin(), _inoutBuffer.end(), inout.begin());
    }



    void
    FFTWPerformer::
    performRealForward(Stream<sample_m>& inout)
    {
        performReal(inout, _realForwardPlan);
        for (size_m i = getTransformSize() / 2; i < getTransformSize(); ++i)
        {
            inout[i] = - inout[i];
        }
    }

    void
    FFTWPerformer::
    performRealInverse(Stream<sample_m>& inout)
    {
        for (size_m i = getTransformSize() / 2; i < getTransformSize(); ++i)
        {
            inout[i] = - inout[i];
        }
        performReal(inout, _realInversePlan);
    }

}
