//
// Created by binabik on 29/05/2020.
//

#include <cant/shift/SoundTouchShifter.hpp>

namespace cant::shift
{
    SoundTouchShifter::
    SoundTouchShifter(const sizeint numberVoices, const sizeint sampleRate, const int_m sequence)
            : _touches(numberVoices)
    {
        for(auto& touch: _touches)
        {
            touch.setSampleRate(sampleRate);
            touch.setChannels(NUMBER_CHANNELS);
            touch.setSetting(SETTING_USE_QUICKSEEK, 1);
            touch.setSetting(SETTING_USE_AA_FILTER, 1);
            /* relative to sequence */
            touch.setSetting(SETTING_SEQUENCE_MS, sequence);
            touch.setSetting(SETTING_SEEKWINDOW_MS, sequence / 2);
            touch.setSetting(SETTING_OVERLAP_MS, sequence / 4);
            /* init values */
            touch.setChannels(1);
            touch.setTempo(1.);
            touch.setPitch(1.);
            touch.setRate(1.);
        }
    }

    void
    SoundTouchShifter::
    clearBuffers(const sizeint iVoice)
    {
        _touches.at(iVoice).clear();
    }

    void
    SoundTouchShifter::
    shift(sizeint iVoice, float_m shiftRatio, const sample_m *input, sample_m *output, sizeint blockSize)
    {
        update(iVoice, input, blockSize);
        auto& touch = _touches.at(iVoice);
        touch.setPitch(shiftRatio);
        touch.receiveSamples(output, blockSize);
    }

    void
    SoundTouchShifter::
    update(sizeint iVoice, const sample_m *input, const sizeint blockSize)
    {
        /** ?? **/
        auto& touch = _touches.at(iVoice);
        touch.putSamples(input, blockSize);
    }
}

