//
// Created by binabik on 03/06/2020.
//

#include <cant/shift/TimeDomainPitchShifter.hpp>

namespace cant::shift
{
    void
    TimeDomainPitchShifter::
    shift(sizeint iVoice, double src, const pan::MidiNoteOutput &data, const sample_m *input,
          sample_m *output, sizeint blockSize)
    {
        CANTINA_TRY_RETHROW({
            if(shouldClearBuffers(data))
            {
                clearBuffers(iVoice);
            }
            else
            {
                const pan::tone_m target = data.getTone();
                const float_m shiftRatio = toneToShiftRatio(src, target);
                shift(iVoice, shiftRatio, input, output, blockSize);
            }
        })
    }
}

