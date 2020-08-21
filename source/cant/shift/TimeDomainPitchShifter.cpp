//
// Created by binabik on 03/06/2020.
//

#include <cant/shift/TimeDomainPitchShifter.hpp>

#include <cant/common/macro.hpp>
namespace cant::shift
{
    void
    TimeDomainPitchShifter::
    shift(double src, const pan::MidiNoteOutput &note, const sample_m *input,
          sample_m *output, size_m blockSize)
    {
        const size_m voice = note.getVoice();
        CANTINA_TRY_RETHROW({
            if(shouldClearBuffers(note))
            {
                clearBuffers(voice);
            }
            else
            {
                const pan::tone_m target = note.getTone();
                const float_m shiftRatio = toneToShiftRatio(src, target);
                shift(voice, shiftRatio, input, output, blockSize);
            }
        })
    }
}

