//
// Created by binabik on 31/05/2020.
//

#include <cant/shift/PitchShifter.hpp>

#include <cmath>
#include <algorithm>

#include <cant/common/CantinaException.hpp>

#include <cant/pan/note/MidiNote.hpp>

#include <cant/common/macro.hpp>
namespace cant::shift
{

    static CANT_CONSTEXPR type_d     s_freqA440          = 440.;
    static CANT_CONSTEXPR pan::tone_d s_toneA440          = 69.;

    static const          type_d      s_twelthRootTwo    = std::pow(2., 1. / 12.);

    static CANT_CONSTEXPR pan::time_d  s_maxLatency       =  40.; // milliseconds
    static CANT_CONSTEXPR pan::time_d  s_preferredLatency =  20.; // milliseconds

    void
    PitchShifter::
    apply(const type_d pitch, const pan::MidiNoteOutput &note, const sample_f *input,
          sample_f *output, size_u blockSize)
    {
        CANTINA_TRY_RETHROW({
            if (shouldClearBuffers(note))
            {
                    clearBuffers(note.getVoice());
            }
            else
            {
                if (shouldTrimBuffers(note, s_maxLatency))
                {
                    trimBuffers(note.getVoice(), timeToNumberSamples(s_preferredLatency));
                }
                shift(freqToTone(pitch), note, input, output, blockSize);
            }
            const volatile pan::vel_d velocityPlaying = note.getVelocityPlaying();
            amplify(output, blockSize, velocityToVolumeRatio(velocityPlaying));
        })
    }

    bool
    PitchShifter::
    shouldClearBuffers(CANT_MAYBEUNUSED const pan::MidiNoteOutput &note) const
    {
        /**
         * BIG TO-DO here!!
         * This ain't good, SoundTouch needs memory of large range of shifting, otherwise it keeps poppin'!!
         * I should clear the buffers ONLY when the output is too far behind the input.
         * First step is to find out how to mesure this latency!!
         **/
         // Well, really not a good idea to clear whole buffers.
         // From now on they'll be trimmed.
        return false;
        // return note.justStopped() && shouldTrimBuffers(note);
    }

    bool
    PitchShifter::
    shouldTrimBuffers(const pan::MidiNoteOutput &note, pan::time_d maxLatency) const
    {
        return !note.isPlaying() && (getLatencyAvailable(note.getVoice()) > maxLatency);
    }

    pan::time_d
    PitchShifter::
    getLatencyAvailable(size_u voice) const
    {
        return static_cast<pan::time_d>(1000 * getNumberSamplesAvailable(voice))
            / static_cast<pan::time_d>(getSampleRate());
    }

    size_u
    PitchShifter::
    timeToNumberSamples(pan::time_d t) const
    {
        return
            static_cast<size_u>(
                    std::round(
                    (t / static_cast<pan::time_d>(1000))
                    * static_cast<pan::time_d>(getSampleRate())
                    )
            );
    }

    type_d
    PitchShifter::
    velocityToVolumeRatio(const pan::vel_d velocity)
    {
        return velocity / static_cast<type_d>(pan::c_midiMaxVelocity);
    }

    void
    PitchShifter::
    amplify(sample_f *block, const size_u blockSize, const type_d amp)
    {
        std::for_each(
                block,
               block + blockSize,
               [amp](sample_f& s) { s *= static_cast<sample_f>(amp); });
    }

    pan::tone_d
    PitchShifter::
    freqToTone(const type_d freq)
    {
        return 12. * (std::log(freq / s_freqA440) / std::log(2)) + s_toneA440;
    }

    type_d
    PitchShifter::
    toneToShiftRatio(const pan::tone_d src, const pan::tone_d dest)
    {
        return std::pow(s_twelthRootTwo, dest - src);
    }
}

