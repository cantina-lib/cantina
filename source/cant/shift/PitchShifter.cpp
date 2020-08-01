//
// Created by binabik on 31/05/2020.
//

#include <cant/shift/PitchShifter.hpp>

#include <cmath>
#include <algorithm>

namespace cant::shift
{
    void
    PitchShifter::
    apply(size_m iVoice, float_m pitch, const pan::MidiNoteOutput &data, const sample_m *input,
          sample_m *output, size_m blockSize)
    {
        CANTINA_TRY_RETHROW({
            shift(iVoice, freqToTone(pitch), data, input, output, blockSize);
            const pan::vel_m velocityPlaying = data.getVelocityPlaying();
            amplify(output, blockSize, velocityToVolumeRatio(velocityPlaying));
        })
    }

    bool
    PitchShifter::
    shouldClearBuffers(const pan::MidiNoteOutput &data)
    {
        return data.justStopped();
    }

    float_m
    PitchShifter::
    velocityToVolumeRatio(const pan::vel_m velocity)
    {
        return (float_m)velocity / pan::MIDI_MAX_VELOCITY;
    }

    void
    PitchShifter::
    amplify(sample_m *block, const size_m blockSize, const float_m amp)
    {
        std::for_each(block, block + blockSize, [amp](sample_m& s) { return s *= amp; });
    }

    pan::tone_m
    PitchShifter::
    freqToTone(const float_m freq)
    {
        return 12. * (std::log(freq / FREQ_A440) / std::log(2)) + TONE_A440;
    }

    float_m
    PitchShifter::
    toneToShiftRatio(const pan::tone_m src, const pan::tone_m dest)
    {
        return std::pow(TWELTH_ROOT_TWO, dest - src);
    }
}

