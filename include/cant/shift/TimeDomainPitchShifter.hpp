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
        shift(pan::tone_m src, const pan::MidiNoteOutput &note, const sample_m *input,
              sample_m *output, size_m blockSize) override;
        virtual void
        shift(size_m voice, float_m shiftRatio, const sample_m *input, sample_m *output, size_m blockSize) = 0;

        void clearBuffers(size_m voice) override = 0;
    private:
        void update(size_m voice, const sample_m *input, size_m blockSize) override = 0;
    };
}

#endif //CANTINA_TIMEDOMAINPITCHSHIFTER_HPP
