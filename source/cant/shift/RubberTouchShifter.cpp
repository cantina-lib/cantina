//
// Created by binabik on 29/05/2020.
//

#include <cant/shift/RubberTouchShifter.hpp>

namespace cant::shift
{
    RubberTouchShifter::
    RubberTouchShifter(UPtr<TimeDomainPitchShifter> auxShifter, size_u numberVoices, size_u sampleRate, size_u blockSize)
    : MixedDomainPitchShifter(std::move(auxShifter)),
      m_bands(
            numberVoices,
            std::move(
                    RubberBand::RubberBandStretcher(
                            sampleRate,
                            c_numberChannels,
                            RubberBand::RubberBandStretcher::OptionProcessRealTime
                            )
            )
    )
    {
        for(auto& band: m_bands)
        {
            band.setMaxProcessSize(blockSize);
        }
    }

    void
    RubberTouchShifter::
    update(const size_u voice, const sample_f *input, const size_u blockSize)
    {
        /* todo */
    }


    void
    RubberTouchShifter::
    clearBuffers(const size_u voice)
    {
        m_bands.at(voice).reset();
    }
    bool
    RubberTouchShifter::
    isReady(const size_u voice) const
    {
        return m_bands.at(voice).available();
    }
}

