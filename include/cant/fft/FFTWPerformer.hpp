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
    public:
        /** -- methods -- **/
        CANT_EXPLICIT FFTWPerformer(size_m transformSize);
        ~FFTWPerformer();
        void performRealForward(Stream<sample_m>& inout);
        void performRealInverse(Stream<sample_m>& inout);

        CANT_NODISCARD size_m getTransformSize() const;
    private:
        /** -- methods -- **/
        void performReal(std::vector<sample_m>& inout, const fftwf_plan& realPlan);

        // static methods
        static fftwf_plan computeRealPlan(Stream<sample_m>& inOutBuffer, fftw_r2r_kind kind);

        /** -- fields -- **/
        fftwf_plan _realForwardPlan, _realInversePlan;
        Stream<sample_m> _inoutBuffer;
    };

}

#include <cant/common/undef_macro.hpp>

#include "FFTWPerformer.inl"

#endif //CANTINA_FFTWANALYSIS_HPP
