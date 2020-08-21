//
// Created by binabik on 31/05/2020.
//

#include <cant/shift/PitchShifter.hpp>

#include <cmath>
#include <algorithm>

#include <cant/common/macro.hpp>
namespace cant::shift
{
    void
    PitchShifter::
    apply(const float_m pitch, const pan::MidiNoteOutput &note, const sample_m *input,
          sample_m *output, size_m blockSize)
    {
        CANTINA_TRY_RETHROW({
            shift(freqToTone(pitch), note, input, output, blockSize);
            const pan::vel_m velocityPlaying = note.getVelocityPlaying();
            amplify(output, blockSize, velocityToVolumeRatio(velocityPlaying));
        })
    }

    bool
    PitchShifter::
    shouldClearBuffers(const pan::MidiNoteOutput &note)
    {
        /**
         * BIG TO-DO here!!
         * This ain't good, SoundTouch needs memory of large range of shifting, otherwise it keeps poppin'!!
         * I should clear the buffers ONLY when the output is too far behind the input.
         * First step is to find out how to mesure this latency!!
         **/
        // return false;
        return note.justStopped();
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
        std::for_each(block, block + blockSize, [amp](sample_m& s) { s *= amp; });
    }

    pan::tone_m
    PitchShifter::
    freqToTone(const float_m freq)
    {
        return 12. * (std::log(freq / s_freqA440) / std::log(2)) + s_toneA440;
    }

    float_m
    PitchShifter::
    toneToShiftRatio(const pan::tone_m src, const pan::tone_m dest)
    {
        return std::pow(s_twelthRootTwo, dest - src);
    }
}

