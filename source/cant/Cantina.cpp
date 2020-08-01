//
// Created by piptouque on 23/04/2020.
//

#include <cant/Cantina.hpp>

#include <cant/pan/pan.hpp>
#include <cant/track/track.hpp>
#include <cant/shift/shift.hpp>

namespace cant
{
    Cantina::
    Cantina(const size_m numberHarmonics,
            const int_m sampleRate,
            const pan::byte_m channelId)
    : _pan(UPtr<pan::Pantoufle>(new pan::Pantoufle(numberHarmonics, channelId))),
      _tracker(UPtr<track::PitchTracker>(new track::HelmholtzTracker(sampleRate))),
      _shifter(UPtr<shift::TimeDomainPitchShifter>(new shift::SoundTouchShifter(numberHarmonics, sampleRate)))
    {

    }

    void
    Cantina::
    perform(const sample_m *in, sample_m *outSeed, sample_m **outHarmonics, const size_m blockSize)
    {
        /* UPDATING CANT */
        CANTINA_TRY_RETHROW({
                update(in, blockSize);
            })
        /* seed */
        std::copy(in, in + blockSize, outSeed);
        /* getting current pitch */
        if(!_tracker->isPitchAcceptable())
        {
            /* do not do anything, return. */
            return;
        }
        float_m pitch = _tracker->getPitchFreq();
        /* getting stream of processed notes */
        const Stream<pan::MidiNoteOutput>& processedNoteOutput = _pan->getProcessedOutputData();
        for(size_m i=0; i < getNumberHarmonics(); ++i)
        {
            const auto& data = processedNoteOutput.at(i);
            sample_m* outHarmonic = outHarmonics[i];
            CANTINA_TRY_RETHROW({
                _shifter->apply(i, pitch, data, in, outHarmonic, blockSize);
            })
        }
    }

    void
    Cantina::
    update(const sample_m* in, const size_m blockSize)
    {

        CANTINA_TRY_RETHROW({
            _tracker->update(in, blockSize);
            // _shifter->update(in);
            _pan->update();
        })
    }

    size_m
    Cantina::
    getNumberHarmonics() const
    {
        return _pan->getNumberVoices();
    }

    void
    Cantina::
    receiveNote(const size_m iVoice, const pan::MidiNoteInputData& noteData)
    {
        PANTOUFLE_TRY_RETHROW({
             _pan->receiveRawNoteData(iVoice, noteData);
        })
    }

    void
    Cantina::
    receiveControl(const pan::MidiControlInputData &controlData)
    {
        CANTINA_TRY_RETHROW({
                _pan->receiveRawControlData(controlData);
        })
    }

    void
    Cantina::
    setController(const std::string &type, const pan::byte_m channelId, const pan::byte_m controllerId)
    {
        if(type == CONTROLLER_TYPE_DAMPER)
        {
            CANTINA_TRY_RETHROW({
                _pan->setController
                        (
                                pan::MidiDamper::make(
                                        getNumberHarmonics(),
                                        channelId,
                                        controllerId
                                )
                        );
            })
        }
        else
        {
            throw CANTINA_EXCEPTION(fmt::format("Controller type not recognised: {0}", type));
        }
    }



}
