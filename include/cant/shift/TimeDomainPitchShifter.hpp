//
// Created by binabik on 03/06/2020.
//

#ifndef CANTINA_TIMEDOMAINPITCHSHIFTER_HPP
#define CANTINA_TIMEDOMAINPITCHSHIFTER_HPP

#pragma once

#include <cant/shift/PitchShifter.hpp>

namespace cant::shift
{
    class TimeDomainPitchShifter : public PitchShifter
    {
    private: // usable only by base class
        void
        shift(sizeint iVoice, midi::tone_m src, const midi::MidiNoteOutput &data, const sample_m *input,
              sample_m *output, sizeint blockSize) override;
        virtual void
        shift(sizeint iVoice, float_m shiftRatio, const sample_m *input, sample_m *output, sizeint blockSize) = 0;
    private:
        void update(sizeint iVoice, const sample_m *input, sizeint blockSize) override = 0;
    };
}

#endif //CANTINA_TIMEDOMAINPITCHSHIFTER_HPP
