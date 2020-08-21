//
// Created by binabik on 29/05/2020.
//

#ifndef CANTINA_RUBBERTOUCHSHIFTER_HPP
#define CANTINA_RUBBERTOUCHSHIFTER_HPP

#pragma once


#include <RubberBandStretcher.h>

#include <cant/common/types.hpp>

#include <cant/shift/MixedDomainPitchShifter.hpp>

#include <cant/common/macro.hpp>
namespace cant::shift
{
    class RubberTouchShifter final : public MixedDomainPitchShifter
    {
    private:
        static const size_m s_maxProcessBatchSize = 1024;
    private:
        std::vector<RubberBand::RubberBandStretcher> _bands;

    private:
        CANT_NODISCARD bool isReady(sizeint iVoice) const override;
    private:
        void update(sizeint iVoice, const sample_m *input, sizeint blockSize) override;
    protected:
        void clearBuffers(sizeint iVoice) override;
    public:
        RubberTouchShifter(UPtr<TimeDomainPitchShifter> auxShifter, sizeint numberVoices, sizeint sampleRate, sizeint blockSize);

    };
}

#include <cant/common/undef_macro.hpp>
#endif //CANTINA_RUBBERTOUCHSHIFTER_HPP
