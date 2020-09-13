//
// Created by binabik on 29/05/2020.
//

#include <cant/shift/SoundTouchShifter.hpp>

#include <cant/common/macro.hpp>
namespace cant::shift
{
    SoundTouchShifter::
    SoundTouchShifter(const size_u numberVoices, const size_u sampleRate, const type_i sequence)
    : m_sampleRate(sampleRate),
      m_touches(numberVoices)
    {
        for(auto& touch: m_touches)
        {
            touch.setSampleRate(static_cast<unsigned int>(sampleRate));
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
        m_touches.at(voice).clear();
    }

    void
    SoundTouchShifter::
    trimBuffers(const size_u voice, const size_u numberSamples)
    {
        m_touches.at(voice).adjustAmountOfSamples(
                static_cast<uint>(numberSamples)
                );
    }

    size_u
    SoundTouchShifter::
    getNumberSamplesAvailable(const size_u voice) const
    {
        return m_touches.at(voice).numSamples();
    }

    size_u
    SoundTouchShifter::
    getSampleRate() const
    {
        return m_sampleRate;
    }

    void
    SoundTouchShifter::
    shift(size_u voice, type_d shiftRatio, const sample_f *input, sample_f *output, size_u blockSize)
    {
        update(voice, input, blockSize);
        auto& touch = m_touches.at(voice);
        touch.setPitch(shiftRatio);
        touch.receiveSamples(output, static_cast<uint>(blockSize));
    }

    void
    SoundTouchShifter::
    update(size_u voice, const sample_f *input, const size_u blockSize)
    {
        /** ?? **/
        auto& touch = m_touches.at(voice);
        touch.putSamples(input, static_cast<uint>(blockSize));
    }

    size_u
    SoundTouchShifter::
    getNumberVoices() const
    {
        return m_touches.size();
    }


}

