//
// Created by binabik on 03/06/2020.
//

#include <cant/shift/MixedDomainPitchShifter.hpp>

#include <cant/common/CantinaException.hpp>

#include <cant/common/macro.hpp>
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
    shift(const cant::float_m src,
          const pan::MidiNoteOutput &data,
          const sample_m *input,
          sample_m *output,
          const size_m blockSize)
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

