//
// Created by binabik on 31/05/2020.
//

#include <cant/shift/PitchShifter.hpp>

#include <cant/common/CantinaException.hpp>
#include <cant/maths/approx.hpp>
#include <cant/reka/common/conversions.hpp>

#include <cant/pan/note/MidiNoteInternalOutput.hpp>

#include <cant/common/macro.hpp>
CANTINA_CANT_NAMESPACE_BEGIN

static CANT_CONSTEXPR time_d c_maxLatency = 40.;       // milliseconds
static CANT_CONSTEXPR time_d c_preferredLatency = 20.; // milliseconds

void PitchShifter::apply(const type_d pitch, const pan::MidiNoteOutput &note,
                         const sample_f *input, sample_f *output,
                         size_u blockSize) {
  CANTINA_TRY_RETHROW({
    if (shouldClearBuffers(note)) {
      clearBuffers(note.getVoice());
    } else {
      if (shouldTrimBuffers(note, c_maxLatency)) {
        trimBuffers(note.getVoice(), timeToNumberSamples(c_preferredLatency));
      }
      shift(reka::Convertor::freqToTone(pitch), note, input, output, blockSize);
    }
    const pan::vel_d velocityPlaying = note.getVelocityPlaying();
    const type_d amp = reka::Convertor::velocityToVolumeRatio(velocityPlaying);
    amplify(output, blockSize, amp);
  })
}

bool PitchShifter::shouldClearBuffers(
    CANT_MAYBEUNUSED const pan::MidiNoteOutput &note) const {
  /**
   * BIG TO-DO here!!
   * This ain't good, SoundTouch needs memory of large range of shifting,
   *otherwise it keeps poppin'!! I should clear the buffers ONLY when the output
   *is too far behind the input. First step is to find out how to mesure this
   *latency!!
   **/
  // Well, really not a good idea to clear whole buffers.
  // From now on they'll be trimmed.
  return false;
  // return note.justStopped() && shouldTrimBuffers(note);
}

bool PitchShifter::shouldTrimBuffers(const pan::MidiNoteOutput &note,
                                     time_d maxLatency) const {
  return !note.isPlaying() &&
         (getLatencyAvailable(note.getVoice()) > maxLatency);
}

time_d PitchShifter::getLatencyAvailable(size_u voice) const {
  return static_cast<time_d>(1000 * getNumberSamplesAvailable(voice)) /
         static_cast<time_d>(getSampleRate());
}

size_u PitchShifter::timeToNumberSamples(time_d t) const {
  return static_cast<size_u>(std::round((t / static_cast<time_d>(1000)) *
                                        static_cast<time_d>(getSampleRate())));
}


void PitchShifter::amplify(sample_f *block, const size_u blockSize,
                           const type_d amp) {
  std::for_each(block, block + blockSize,
                [amp](sample_f &s) { s *= static_cast<sample_f>(amp); });
}


type_d PitchShifter::toneToShiftRatio(const pan::tone_d src,
                                      const pan::tone_d dest) {
  static const type_d twelthRootTwo = std::pow(2., 1. / 12.);
  return std::pow(twelthRootTwo, dest - src);
}

CANTINA_CANT_NAMESPACE_END
