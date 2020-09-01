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
    class TimeDomainPitchShifter;

    class RubberTouchShifter final : public MixedDomainPitchShifter
    {
    public:
        /** -- methods -- **/
        RubberTouchShifter(UPtr<TimeDomainPitchShifter> auxShifter, sizeint numberVoices, sizeint sampleRate, sizeint blockSize);
    protected:
        /** -- methods -- **/
        void clearBuffers(sizeint iVoice) override;
        void update(size_m iVoice, const sample_m *input, size_m blockSize) override;

        CANT_NODISCARD bool isReady(size_m iVoice) const override;

        /** -- fields -- **/
        std::vector<RubberBand::RubberBandStretcher> _bands;

        // static fields
        static CANT_CONSTEXPR size_m s_maxProcessBatchSize = 1024;
    };
}

#include <cant/common/undef_macro.hpp>
#endif //CANTINA_RUBBERTOUCHSHIFTER_HPP
