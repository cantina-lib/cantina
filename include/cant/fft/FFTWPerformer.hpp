//
// Created by binabik on 26/07/2020.
//

#ifndef CANTINA_FFTWANALYSIS_HPP
#define CANTINA_FFTWANALYSIS_HPP

#include <fftw3.h>

#include <cant/common/types.hpp>

namespace cant::fft
{
    class FFTWPerformer
    {
    private:
        sample_m* _inoutBuffer;
        fftwf_plan _forwardPlan, _inversePlan;
        sizeint _blockSize;
    private:
        void perform(sample_m* inout, sizeint blockSize, const fftwf_plan& plan);
    public:
        CANT_EXPLICIT FFTWPerformer(sizeint blockSize);
        ~FFTWPerformer();
        void performForward(sample_m* inout, sizeint blockSize);
        void performInverse(sample_m* inout, sizeint blockSize);
    };

}

#endif //CANTINA_FFTWANALYSIS_HPP
