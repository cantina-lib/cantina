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
    FFTWPerformer(const size_u transformSize)
        : m_inoutBuffer(transformSize),
        m_realForwardPlan(computeRealPlan(m_inoutBuffer, FFTW_R2HC)),
        m_realInversePlan(computeRealPlan(m_inoutBuffer, FFTW_HC2R))
    {
    }

    fftwf_plan
    FFTWPerformer::
    computeRealPlan(FFTW3FBlock &inOutBuffer, fftw_r2r_kind kind)
    {
        return fftwf_plan_r2r_1d(
                static_cast<int>(inOutBuffer.size()),
                inOutBuffer.data(),
                inOutBuffer.data(),
                kind,
                FFTW_PATIENT
                );
    }

    FFTWPerformer::
    ~FFTWPerformer()
    {
        fftwf_destroy_plan(m_realForwardPlan);
        fftwf_destroy_plan(m_realInversePlan);
    }

    void
    FFTWPerformer::
    performReal(Stream <sample_f> &inout, const fftwf_plan& realPlan)
    {
        /*
         * the received sample has not been allocated with fftwf_malloc,
         * so for alignment reasons, we should process from
         * the internal _inoutBuffer
         * then copy the result.
         */
        CANTINA_ASSERT(inout.size() == getTransformSize(), "Nope nope nope.");
        std::copy(inout.begin(), inout.end(), m_inoutBuffer.begin());
        fftwf_execute(realPlan);
        std::copy(m_inoutBuffer.begin(), m_inoutBuffer.end(), inout.begin());
    }



    void
    FFTWPerformer::
    performRealForward(Stream <sample_f> &inout)
    {
        performReal(inout, m_realForwardPlan);
        for (size_u i = getTransformSize() / 2; i < getTransformSize(); ++i)
        {
            inout.at(i) = - inout.at(i);
        }
    }

    void
    FFTWPerformer::
    performRealInverse(Stream <sample_f> &inout)
    {
        for (size_u i = getTransformSize() / 2; i < getTransformSize(); ++i)
        {
            inout.at(i) = - inout.at(i);
        }
        performReal(inout, m_realInversePlan);
    }

}
