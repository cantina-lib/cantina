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

    class PitchShifter
    {
    public:
        /** -- methods -- **/
        void apply(
                float_m pitch,
                const pan::MidiNoteOutput &note,
                const sample_m *input,
                sample_m *output,
                size_m blockSize
        );

    protected:
        /** -- methods -- **/
        CANT_NODISCARD bool shouldClearBuffers(const pan::MidiNoteOutput &note) const;
        CANT_NODISCARD bool shouldTrimBuffers(const pan::MidiNoteOutput &note, pan::time_m maxLatency) const;

        // In milliseconds
        CANT_NODISCARD pan::time_m getLatencyAvailable(size_m voice) const;
        // In milliseconds again
        CANT_NODISCARD size_m timeToNumberSamples(pan::time_m t) const;

        CANT_NODISCARD virtual size_m getNumberSamplesAvailable(size_m voice) const = 0;
        CANT_NODISCARD virtual size_m getSampleRate() const = 0;

        // static methods
        static float_m toneToShiftRatio(pan::tone_m src, pan::tone_m dest);

        /** -- fields --**/
        // constants
        static CANT_CONSTEXPR int_m       c_numberChannels    = 1;
    private:
        /** -- methods -- **/
        virtual void
        shift(pan::tone_m src, const pan::MidiNoteOutput &data, const sample_m *input,
              sample_m *output, size_m blockSize) = 0;

        virtual void update(size_m voice, const sample_m *input, size_m blockSize) = 0;

        virtual void clearBuffers(size_m voice) = 0;
        virtual void trimBuffers(size_m voice, size_m numberSamples) = 0;

        // static methods
        static float_m velocityToVolumeRatio(pan::vel_m velocity);
        static pan::tone_m freqToTone(float_m freq);
        static void amplify(sample_m *block, size_m blockSize, float_m amp);
    };



}

#include <cant/common/undef_macro.hpp>
#endif //CANTINA_PITCHSHIFTER_HPP
