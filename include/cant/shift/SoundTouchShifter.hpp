//
// Created by binabik on 29/05/2020.
//

#ifndef CANTINA_SOUNDTOUCHSHIFTER_HPP
#define CANTINA_SOUNDTOUCHSHIFTER_HPP

#pragma once

#include <SoundTouch.h>


#include <cant/common/types.hpp>
#include <cant/shift/TimeDomainPitchShifter.hpp>

#include <cant/common/macro.hpp>
namespace cant::shift
{
    class SoundTouchShifter final : public TimeDomainPitchShifter
    {
    public:
        /** -- methods -- **/
        SoundTouchShifter(size_u numberVoices, type_i sampleRate, type_i sequence = c_defaultSequence);

        CANT_NODISCARD size_u getNumberVoices() const;
    protected:
        /** -- methods -- **/
        CANT_NODISCARD size_u getNumberSamplesAvailable(size_u voice) const override;
        CANT_NODISCARD size_u getSampleRate() const override;

    private: // unsable here, only in base class
        /** -- methods -- **/
        // private inheritance
        void update(size_u voice, const sample_f *input, size_u blockSize) override;
        void clearBuffers(size_u voice) override;
        void trimBuffers(size_u voice, size_u numberSamples) override;
        void shift(size_u voice, type_d shiftRatio, const sample_f *input, sample_f *output, size_u blockSize) override;


        /** -- fields -- **/
        size_u m_sampleRate;
        std::vector<soundtouch::SoundTouch> m_touches;

        // constants
        static CANT_CONSTEXPR type_i c_defaultSequence = 20;
    };
}

#include <cant/common/undef_macro.hpp>
#endif //CANTINA_SOUNDTOUCHSHIFTER_HPP
