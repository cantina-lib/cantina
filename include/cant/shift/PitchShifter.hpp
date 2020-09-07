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
                type_d pitch,
                const pan::MidiNoteOutput &note,
                const sample_f *input,
                sample_f *output,
                size_u blockSize
        );

    protected:
        /** -- methods -- **/
        CANT_NODISCARD bool shouldClearBuffers(const pan::MidiNoteOutput &note) const;
        CANT_NODISCARD bool shouldTrimBuffers(const pan::MidiNoteOutput &note, pan::time_d maxLatency) const;

        // In milliseconds
        CANT_NODISCARD pan::time_d getLatencyAvailable(size_u voice) const;
        // In milliseconds again
        CANT_NODISCARD size_u timeToNumberSamples(pan::time_d t) const;

        CANT_NODISCARD virtual size_u getNumberSamplesAvailable(size_u voice) const = 0;
        CANT_NODISCARD virtual size_u getSampleRate() const = 0;

        // static methods
        static type_d toneToShiftRatio(pan::tone_d src, pan::tone_d dest);

        /** -- fields --**/
        // constants
        static CANT_CONSTEXPR type_i       c_numberChannels    = 1;
    private:
        /** -- methods -- **/
        virtual void
        shift(pan::tone_d src, const pan::MidiNoteOutput &data, const sample_f *input,
              sample_f *output, size_u blockSize) = 0;

        virtual void update(size_u voice, const sample_f *input, size_u blockSize) = 0;

        virtual void clearBuffers(size_u voice) = 0;
        virtual void trimBuffers(size_u voice, size_u numberSamples) = 0;

        // static methods
        static type_d velocityToVolumeRatio(pan::vel_d velocity);
        static pan::tone_d freqToTone(type_d freq);
        static void amplify(sample_f *block, size_u blockSize, type_d amp);
    };



}

#include <cant/common/undef_macro.hpp>
#endif //CANTINA_PITCHSHIFTER_HPP
