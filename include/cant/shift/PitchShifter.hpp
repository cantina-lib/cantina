//
// Created by piptouque on 01/05/2020.
//

#ifndef CANTINA_PITCHSHIFTER_HPP
#define CANTINA_PITCHSHIFTER_HPP

#pragma once


#include <cant/common/types.hpp>
#include <cant/common/memory.hpp>

#include <cant/maths/maths.hpp>

#include <cant/pan/common/types.hpp>

#include <cant/pan/note/MidiNote.hpp>

#include <cant/common/macro.hpp>
namespace cant::shift
{
    static CANT_CONSTEXPR int_m       s_numberChannels    = 1;

    static CANT_CONSTEXPR float_m     s_freqA440          = 440.;
    static CANT_CONSTEXPR pan::tone_m s_toneA440          = 69.;

    static const          float_m      s_twelthRootTwo    = std::pow(2., 1. / 12.);

    static CANT_CONSTEXPR pan::time_m  s_maxLatency       =  40.; // milliseconds
    static CANT_CONSTEXPR pan::time_m  s_preferredLatency =  20.; // milliseconds

    class PitchShifter
    {
    private:
        virtual void
        shift(pan::tone_m src, const pan::MidiNoteOutput &data, const sample_m *input,
              sample_m *output, size_m blockSize) = 0;

        virtual void update(size_m voice, const sample_m *input, size_m blockSize) = 0;

        virtual void clearBuffers(size_m voice) = 0;
        virtual void trimBuffers(size_m voice, size_m numberSamples) = 0;
    private:
        static float_m velocityToVolumeRatio(pan::vel_m velocity);
        static pan::tone_m freqToTone(float_m freq);
        static void amplify(sample_m *block, size_m blockSize, float_m amp);
    protected:
        CANT_NODISCARD bool shouldClearBuffers(const pan::MidiNoteOutput &note) const;
        CANT_NODISCARD bool shouldTrimBuffers(const pan::MidiNoteOutput &note, pan::time_m maxLatency) const;


        static float_m toneToShiftRatio(pan::tone_m src, pan::tone_m dest);

        // In milliseconds
        CANT_NODISCARD pan::time_m getLatencyAvailable(size_m voice) const;

        // In milliseconds again
        CANT_NODISCARD size_m timeToNumberSamples(pan::time_m t) const;

        CANT_NODISCARD virtual size_m getNumberSamplesAvailable(size_m voice) const = 0;

        CANT_NODISCARD virtual size_m getSampleRate() const = 0;
    public:
       void apply(
               float_m pitch,
               const pan::MidiNoteOutput &note,
               const sample_m *input,
               sample_m *output,
               size_m blockSize
               );
    };



}

#include <cant/common/undef_macro.hpp>
#endif //CANTINA_PITCHSHIFTER_HPP
