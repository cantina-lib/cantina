//
// Created by binabik on 03/06/2020.
//

#include <cant/shift/MixedDomainPitchShifter.hpp>

namespace cant::shift
{

    MixedDomainPitchShifter::
    MixedDomainPitchShifter(UPtr<TimeDomainPitchShifter>&& auxShifter)
    : _auxShifter(std::move(auxShifter))
    {
        if(!_auxShifter)
        {
            throw CANTINA_EXCEPTION("Auxiliary shifter not set.");
        }
    }

    void
    MixedDomainPitchShifter::
    shift(size_m iVoice, double src, const pan::MidiNoteOutput &data, const sample_m *input,
          sample_m *output, size_m blockSize)
    {
        /*
        auto& band = _bands.at(iVoice);
        band.setPitchScale(shiftRatio);

        band.process(&input, blockSize, true);
        if(band.available())
        {
            band.retrieve(&output, blockSize);
        }
        */
    }

}

