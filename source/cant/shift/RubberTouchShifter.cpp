//
// Created by binabik on 29/05/2020.
//

#include <cant/shift/RubberTouchShifter.hpp>

namespace cant::shift
{
    RubberTouchShifter::
    RubberTouchShifter(UPtr<TimeDomainPitchShifter> auxShifter, size_m numberVoices, size_m sampleRate, size_m blockSize)
    : MixedDomainPitchShifter(std::move(auxShifter)),
      _bands(
            numberVoices,
            std::move(RubberBand::RubberBandStretcher(sampleRate, NUMBER_CHANNELS, RubberBand::RubberBandStretcher::OptionProcessRealTime))
    )
    {
        for(auto& band: _bands)
        {
            band.setMaxProcessSize(blockSize);
        }
    }

    void
    RubberTouchShifter::
    update(const size_m iVoice, const sample_m *input, const size_m blockSize)
    {
        /* todo */
    }


    void
    RubberTouchShifter::
    clearBuffers(const size_m iVoice)
    {
        _bands.at(iVoice).reset();
    }
    bool
    RubberTouchShifter::
    isReady(const size_m iVoice) const
    {
        return _bands.at(iVoice).available();
    }
}

