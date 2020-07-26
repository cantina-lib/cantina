//
// Created by binabik on 29/05/2020.
//

#ifndef CANTINA_SOUNDTOUCHSHIFTER_HPP
#define CANTINA_SOUNDTOUCHSHIFTER_HPP

#pragma once

#include <SoundTouch.h>

#include <cant/common/types.hpp>
#include <cant/shift/TimeDomainPitchShifter.hpp>

namespace cant::shift
{
    class SoundTouchShifter final : public TimeDomainPitchShifter
    {
    private:
        static constexpr int_m m_DEFAULT_SEQUENCE = 20;
    private:
        std::vector<soundtouch::SoundTouch> _touches;
    private: // unsable here, only in base class
        void update(sizeint iVoice, const sample_m *input, sizeint blockSize) override;
        void clearBuffers(sizeint iVoice) override;
        void shift(sizeint iVoice, float_m shiftRatio, const sample_m *input, sample_m *output,
                   sizeint blockSize) override;
    public:
        SoundTouchShifter(sizeint numberVoices, sizeint sampleRate, int_m sequence = m_DEFAULT_SEQUENCE);
    };
}

#endif //CANTINA_SOUNDTOUCHSHIFTER_HPP
