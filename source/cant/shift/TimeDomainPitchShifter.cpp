//
// Created by binabik on 03/06/2020.
//

#include <cant/shift/TimeDomainPitchShifter.hpp>

#include <cant/pan/note/MidiNoteInternalOutput.hpp>

#include <cant/common/macro.hpp>
CANTINA_CANT_NAMESPACE_BEGIN

void TimeDomainPitchShifter::shift(type_d src, const pan::MidiNoteOutput &note,
                                   const sample_f *input, sample_f *output,
                                   size_u blockSize) {
  const pan::tone_d target = note.getTone();
  const type_d shiftRatio = toneToShiftRatio(src, target);
    shiftVoice(note.getVoice(), shiftRatio, input, output, blockSize);
}

CANTINA_CANT_NAMESPACE_END
