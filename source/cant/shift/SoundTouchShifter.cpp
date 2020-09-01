//
// Created by binabik on 29/05/2020.
//

#include <cant/shift/SoundTouchShifter.hpp>

#include <cant/common/macro.hpp>
namespace cant::shift
{
    SoundTouchShifter::
    SoundTouchShifter(const size_m numberVoices, const int_m sampleRate, const int_m sequence)
    : _sampleRate(sampleRate),
    _touches(numberVoices)
    {
        for(auto& touch: _touches)
        {
            touch.setSampleRate(sampleRate);
            touch.setChannels(c_numberChannels);
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
    clearBuffers(const size_m voice)
    {
        _touches.at(voice).clear();
    }

    void
    SoundTouchShifter::
    trimBuffers(const size_m voice, const size_m numberSamples)
    {
        // std::cout << "yeah " << getNumberSamplesAvailable(voice);
        CANT_MAYBEUNUSED const size_m trimmedBufferSize = _touches.at(voice).adjustAmountOfSamples(numberSamples);
        // std::cout << "no " << trimmedBufferSize << std::endl;
    }

    size_m
    SoundTouchShifter::
    getNumberSamplesAvailable(const size_m voice) const
    {
        return _touches.at(voice).numSamples();
    }

    size_m
    SoundTouchShifter::
    getSampleRate() const
    {
        return _sampleRate;
    }

    void
    SoundTouchShifter::
    shift(size_m iVoice, float_m shiftRatio, const sample_m *input, sample_m *output, size_m blockSize)
    {
        update(iVoice, input, blockSize);
        auto& touch = _touches.at(iVoice);
        touch.setPitch(shiftRatio);
        touch.receiveSamples(output, blockSize);
    }

    void
    SoundTouchShifter::
    update(size_m iVoice, const sample_m *input, const size_m blockSize)
    {
        /** ?? **/
        auto& touch = _touches.at(iVoice);
        touch.putSamples(input, blockSize);
    }

    size_m
    SoundTouchShifter::
    getNumberVoices() const
    {
        return _touches.size();
    }


}

