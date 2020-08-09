//
// Created by binabik on 03/06/2020.
//

#ifndef CANTINA_MIXEDDOMAINPITCHSHIFTER_HPP
#define CANTINA_MIXEDDOMAINPITCHSHIFTER_HPP

#pragma once

#include <cant/pan/common/types.hpp>

#include <cant/shift/PitchShifter.hpp>
#include <cant/shift/TimeDomainPitchShifter.hpp>

namespace cant::shift
{
    class MixedDomainPitchShifter : public PitchShifter
    {
    private:
       UPtr<TimeDomainPitchShifter> _auxShifter;
    private:
        void
        shift(
                pan::tone_m src,
                const pan::MidiNoteOutput &note,
                const sample_m *input,
                sample_m *output,
                size_m blockSize
                ) override;
        CANT_NODISCARD virtual bool isReady(const pan::MidiNoteOutput& note) const = 0;
    public:
        explicit MixedDomainPitchShifter(UPtr<TimeDomainPitchShifter>&& auxShifter);
    };
}

#endif //CANTINA_MIXEDDOMAINPITCHSHIFTER_HPP
