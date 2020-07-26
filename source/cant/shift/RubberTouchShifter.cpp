//
// Created by binabik on 29/05/2020.
//

#include <cant/shift/RubberTouchShifter.hpp>

namespace cant::shift
{
    RubberTouchShifter::
    RubberTouchShifter(UPtr<TimeDomainPitchShifter>&& auxShifter, const sizeint numberVoices, const sizeint sampleRate, const sizeint blockSize)
    : MixedDomainPitchShifter(std::move(auxShifter)),
      _bands(
            numberVoices,
            RubberBand::RubberBandStretcher(sampleRate, NUMBER_CHANNELS, RubberBand::RubberBandStretcher::OptionProcessRealTime)
    )
    {
        for(auto& band: _bands)
        {
            band.setMaxProcessSize(blockSize);
        }
    }

    void
    RubberTouchShifter::
    update(const sizeint iVoice, const sample_m *input, const sizeint blockSize)
    {
        /* todo */
    }


    void
    RubberTouchShifter::
    clearBuffers(const sizeint iVoice)
    {
        _bands.at(iVoice).reset();
    }
    bool
    RubberTouchShifter::
    isReady(const sizeint iVoice) const
    {
        return _bands.at(iVoice).available();
    }
}

