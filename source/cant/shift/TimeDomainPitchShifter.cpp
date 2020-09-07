//
// Created by binabik on 03/06/2020.
//

#include <cant/shift/TimeDomainPitchShifter.hpp>

#include <cant/common/CantinaException.hpp>

#include <cant/common/macro.hpp>
namespace cant::shift
{
    void
    TimeDomainPitchShifter::
    shift(double src, const pan::MidiNoteOutput &note, const sample_f *input,
          sample_f *output, size_u blockSize)
    {
        CANTINA_TRY_RETHROW({
            const pan::tone_d target = note.getTone();
            const type_d shiftRatio = toneToShiftRatio(src, target);
            shift(note.getVoice(), shiftRatio, input, output, blockSize);
        })
    }
}

