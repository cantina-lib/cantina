//
// Created by binabik on 29/05/2020.
//

#include <cant/shift/SoundTouchShifter.hpp>

#include <cant/common/macro.hpp>
namespace cant::shift
{
    SoundTouchShifter::
    SoundTouchShifter(const size_u numberVoices, const type_i sampleRate, const type_i sequence)
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
    clearBuffers(const size_u voice)
    {
        _touches.at(voice).clear();
    }

    void
    SoundTouchShifter::
    trimBuffers(const size_u voice, const size_u numberSamples)
    {
        // std::cout << "yeah " << getNumberSamplesAvailable(voice);
        CANT_MAYBEUNUSED const size_u trimmedBufferSize = _touches.at(voice).adjustAmountOfSamples(numberSamples);
        // std::cout << "no " << trimmedBufferSize << std::endl;
    }

    size_u
    SoundTouchShifter::
    getNumberSamplesAvailable(const size_u voice) const
    {
        return _touches.at(voice).numSamples();
    }

    size_u
    SoundTouchShifter::
    getSampleRate() const
    {
        return _sampleRate;
    }

    void
    SoundTouchShifter::
    shift(size_u iVoice, type_d shiftRatio, const sample_f *input, sample_f *output, size_u blockSize)
    {
        update(iVoice, input, blockSize);
        auto& touch = _touches.at(iVoice);
        touch.setPitch(shiftRatio);
        touch.receiveSamples(output, blockSize);
    }

    void
    SoundTouchShifter::
    update(size_u iVoice, const sample_f *input, const size_u blockSize)
    {
        /** ?? **/
        auto& touch = _touches.at(iVoice);
        touch.putSamples(input, blockSize);
    }

    size_u
    SoundTouchShifter::
    getNumberVoices() const
    {
        return _touches.size();
    }


}

