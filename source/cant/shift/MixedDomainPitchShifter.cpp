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
    shift(CANT_MAYBEUNUSED const cant::type_d src,
          CANT_MAYBEUNUSED const pan::MidiNoteOutput &data,
          CANT_MAYBEUNUSED const sample_f *input,
          CANT_MAYBEUNUSED sample_f *output,
          CANT_MAYBEUNUSED const size_u blockSize)
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

