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
        void update(size_m voice, const sample_m *input, size_m blockSize) override;
        void clearBuffers(size_m voice) override;
        void shift(size_m voice, float_m shiftRatio, const sample_m *input, sample_m *output, size_m blockSize) override;
    public:
        SoundTouchShifter(size_m numberVoices, int_m sampleRate, int_m sequence = m_DEFAULT_SEQUENCE);

        CANT_NODISCARD size_m getNumberVoices() const;
    };
}

#endif //CANTINA_SOUNDTOUCHSHIFTER_HPP
