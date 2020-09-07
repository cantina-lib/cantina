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
        CANT_EXPLICIT FFTWPerformer(size_u transformSize);
        ~FFTWPerformer();
        void performRealForward(Stream<sample_f>& inout);
        void performRealInverse(Stream<sample_f>& inout);

        CANT_NODISCARD size_u getTransformSize() const;
    private:
        /** -- methods -- **/
        void performReal(std::vector<sample_f>& inout, const fftwf_plan& realPlan);

        // static methods
        static fftwf_plan computeRealPlan(Stream<sample_f>& inOutBuffer, fftw_r2r_kind kind);

        /** -- fields -- **/
        fftwf_plan _realForwardPlan, _realInversePlan;
        Stream<sample_f> _inoutBuffer;
    };

}

#include <cant/common/undef_macro.hpp>

#include "FFTWPerformer.inl"

#endif //CANTINA_FFTWANALYSIS_HPP
