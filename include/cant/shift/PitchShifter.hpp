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
    static constexpr midi::tone_m TONE_A440 = 69.;
    static const float_m TWELTH_ROOT_TWO = std::pow(2., 1./12.);

    class PitchShifter
    {
    private:
        virtual void update(sizeint iVoice, const sample_m *input, sizeint blockSize) = 0;
        virtual void
        shift(sizeint iVoice, midi::tone_m src, const midi::MidiNoteOutput &data, const sample_m *input,
              sample_m *output, sizeint blockSize) = 0;
    private:
        static float_m velocityToVolumeRatio(midi::vel_m velocity);
        static midi::tone_m freqToTone(float_m freq);
        static void amplify(sample_m *block, sizeint blockSize, float_m amp);
    protected:
        static bool shouldClearBuffers(const midi::MidiNoteOutput &data);

        virtual void clearBuffers(sizeint iVoice) = 0;
        static float_m toneToShiftRatio(midi::tone_m src, midi::tone_m dest);
    public:
       void apply(sizeint iVoice, float_m pitch, const midi::MidiNoteOutput &data, const sample_m *input,
                  sample_m *output, sizeint blockSize);
    };



}

#endif //CANTINA_PITCHSHIFTER_HPP
