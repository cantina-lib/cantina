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

#include <vector>

namespace cant::fft
{
    class FFTWPerformer
    {
    private:
        fftwf_plan _realForwardPlan, _realInversePlan;

        std::vector<sample_m> _inoutBuffer;
    private:
        void performReal(std::vector<sample_m>& inout, const fftwf_plan& realPlan);

        static fftwf_plan computeRealPlan(std::vector<sample_m>& inOutBuffer, fftw_r2r_kind kind);

    public:
        CANT_EXPLICIT FFTWPerformer(sizeint transformSize);
        ~FFTWPerformer();
        void performRealForward(std::vector<sample_m>& inout);
        void performRealInverse(std::vector<sample_m>& inout);

        CANT_NODISCARD sizeint getTransformSize() const { return _inoutBuffer.size(); }

    };

}

#endif //CANTINA_FFTWANALYSIS_HPP
