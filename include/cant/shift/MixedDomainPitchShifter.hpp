//
// Created by binabik on 03/06/2020.
//

#ifndef CANTINA_MIXEDDOMAINPITCHSHIFTER_HPP
#define CANTINA_MIXEDDOMAINPITCHSHIFTER_HPP

#pragma once

#include <cant/pan/common/types.hpp>

#include <cant/shift/PitchShifter.hpp>
#include <cant/shift/TimeDomainPitchShifter.hpp>

#include <cant/common/macro.hpp>
namespace cant::shift
{
    class MixedDomainPitchShifter : public PitchShifter
    {
    public:
        /** -- methods -- **/
        CANT_EXPLICIT
        MixedDomainPitchShifter(UPtr<TimeDomainPitchShifter>&& auxShifter);
    private:
        /** -- methods -- **/
        void
        shift(
                pan::tone_d src,
                const pan::MidiNoteOutput &note,
                const sample_f *input,
                sample_f *output,
                size_u blockSize
        ) override;

        CANT_NODISCARD virtual bool isReady(const pan::MidiNoteOutput& note) const = 0;

        /** -- fields -- **/
        UPtr<TimeDomainPitchShifter> _auxShifter;
    };
}

#include <cant/common/undef_macro.hpp>
#endif //CANTINA_MIXEDDOMAINPITCHSHIFTER_HPP
