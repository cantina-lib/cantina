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
    void
    PitchShifter::
    apply(const float_m pitch, const pan::MidiNoteOutput &note, const sample_m *input,
          sample_m *output, size_m blockSize)
    {
        CANTINA_TRY_RETHROW({
                                shift(freqToTone(pitch), note, input, output, blockSize);
                                const pan::vel_m velocityPlaying = note.getVelocityPlaying();
                                amplify(output, blockSize, velocityToVolumeRatio(velocityPlaying));
                            })
    }

    bool
    PitchShifter::
    shouldClearBuffers(const pan::MidiNoteOutput &note)
    {
        return note.justStopped();
    }

    float_m
    PitchShifter::
    velocityToVolumeRatio(const pan::vel_m velocity)
    {
        return (float_m)velocity / pan::MIDI_MAX_VELOCITY;
    }

    void
    PitchShifter::
    amplify(sample_m *block, const size_m blockSize, const float_m amp)
    {
        std::for_each(block, block + blockSize, [amp](sample_m& s) { s *= amp; });
    }

    pan::tone_m
    PitchShifter::
    freqToTone(const float_m freq)
    {
        return 12. * (std::log(freq / s_freqA440) / std::log(2)) + s_toneA440;
    }

    float_m
    PitchShifter::
    toneToShiftRatio(const pan::tone_m src, const pan::tone_m dest)
    {
        return std::pow(s_twelthRootTwo, dest - src);
    }
}

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

#include <cant/common/undef_macro.hpp>
#endif //CANTINA_MIXEDDOMAINPITCHSHIFTER_HPP
