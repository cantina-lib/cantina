//
// Created by piptouque on 01/05/2020.
//

#ifndef CANTINA_PITCHSHIFTER_HPP
#define CANTINA_PITCHSHIFTER_HPP

#pragma once


#include <cant/common/types.hpp>

#include <cant/pan/pan.hpp>

namespace cant::shift
{
    static constexpr int_m NUMBER_CHANNELS = 1;

    static constexpr float_m FREQ_A440 = 440.;
    static constexpr pan::tone_m TONE_A440 = 69.;
    static const float_m TWELTH_ROOT_TWO = std::pow(2., 1./12.);

    class PitchShifter
    {
    private:
        virtual void update(size_m iVoice, const sample_m *input, size_m blockSize) = 0;
        virtual void
        shift(size_m iVoice, pan::tone_m src, const pan::MidiNoteOutput &data, const sample_m *input,
              sample_m *output, size_m blockSize) = 0;
    private:
        static float_m velocityToVolumeRatio(pan::vel_m velocity);
        static pan::tone_m freqToTone(float_m freq);
        static void amplify(sample_m *block, size_m blockSize, float_m amp);
    protected:
        static bool shouldClearBuffers(const pan::MidiNoteOutput &output);

        virtual void clearBuffers(size_m iVoice) = 0;
        static float_m toneToShiftRatio(pan::tone_m src, pan::tone_m dest);
    public:
       void apply(
               size_m iVoice,
               float_m pitch,
               const pan::MidiNoteOutput &note,
               const sample_m *input,
               sample_m *output,
               size_m blockSize
               );
    };



}

#endif //CANTINA_PITCHSHIFTER_HPP
