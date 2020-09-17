//
// Created by binabik on 03/06/2020.
//

#include <cant/shift/TimeDomainPitchShifter.hpp>

#include <cant/common/CantinaException.hpp>

#include <cant/pan/note/MidiNote.hpp>

#include <cant/common/macro.hpp>
CANTINA_CANT_NAMESPACE_BEGIN

    void
    TimeDomainPitchShifter::
    shift
    (
            type_d src,
            const CANTINA_PAN_NAMESPACE::MidiNoteOutput &note,
            const sample_f *input,
            sample_f *output,
            size_u blockSize
    )
    {
        CANTINA_TRY_RETHROW({
            const CANTINA_PAN_NAMESPACE::tone_d target = note.getTone();
            const type_d shiftRatio = toneToShiftRatio(src, target);
            shift(note.getVoice(), shiftRatio, input, output, blockSize);
        })
    }

CANTINA_CANT_NAMESPACE_END
