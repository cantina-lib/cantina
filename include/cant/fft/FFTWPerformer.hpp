//
// Created by binabik on 26/07/2020.
//

#ifndef CANTINA_FFTWANALYSIS_HPP
#define CANTINA_FFTWANALYSIS_HPP

#pragma once

extern "C"
{
#include <fftw3.h>
};

#include <cant/common/types.hpp>
#include <cant/common/memory.hpp>

#include <cant/common/macro.hpp>
namespace cant::fft
{
    class FFTWPerformer
    {
    private:
        fftwf_plan _realForwardPlan, _realInversePlan;

        Stream<sample_m> _inoutBuffer;
    private:
        void performReal(std::vector<sample_m>& inout, const fftwf_plan& realPlan);

        static fftwf_plan computeRealPlan(Stream<sample_m>& inOutBuffer, fftw_r2r_kind kind);

    public:
        CANT_EXPLICIT FFTWPerformer(size_m transformSize);
        ~FFTWPerformer();
        void performRealForward(Stream<sample_m>& inout);
        void performRealInverse(Stream<sample_m>& inout);

        CANT_NODISCARD size_m getTransformSize() const { return _inoutBuffer.size(); }

    };

}

#include <cant/common/undef_macro.hpp>
#endif //CANTINA_FFTWANALYSIS_HPP
