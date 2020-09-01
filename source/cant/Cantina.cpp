//
// Created by piptouque on 23/04/2020.
//

#include <cant/Cantina.hpp>
#include <cant/common/config.hpp>

#include <cant/pan/pan.hpp>
#include <cant/pan/time/MidiTimer.hpp>

#include <cant/track/HelmholtzTracker.hpp>
#include <cant/shift/SoundTouchShifter.hpp>

#include <cant/common/CantinaException.hpp>

#include <cant/common/macro.hpp>
namespace cant
{
    Cantina::
    Cantina(const size_m numberHarmonics,
            const int_m sampleRate,
            const pan::byte_m channelId)
    : _pantoufle(std::make_unique<pan::Pantoufle>(numberHarmonics, channelId)),
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
        const Stream<pan::MidiNoteOutput>& processedNoteOutput = _pantoufle->getProcessedOutputData();
        for(size_m i=0; i < getNumberHarmonics(); ++i)
        {
            const auto& note = processedNoteOutput.at(i);
            sample_m* outHarmonic = outHarmonics[i];
            CANTINA_TRY_RETHROW({
                _shifter->apply(pitch, note, in, outHarmonic, blockSize);
            })
        }
    }

    void
    Cantina::
    update(const sample_m* in, const size_m blockSize)
    {

        CANTINA_TRY_RETHROW({
            _tracker->update(in, blockSize);
            _pantoufle->update();
        })
    }

    size_m
    Cantina::
    getNumberHarmonics() const
    {
        return _pantoufle->getNumberVoices();
    }

    void
    Cantina::
    receiveNote(const pan::MidiNoteInputData& noteData)
    {
        CANTINA_TRY_RETHROW({
          _pantoufle->receiveInputNoteData(noteData);
        })
    }

    void
    Cantina::
    receiveControl(const pan::MidiControlInputData &controlData)
    {
        CANTINA_TRY_RETHROW({
                _pantoufle->receiveRawControlData(controlData);
        })
    }

    void
    Cantina::
    setController(const std::string &type, const pan::byte_m channel, const Stream <pan::byte_m> &controllerIds)
    {
        if (type == CONTROLLER_TYPE_DAMPER)
        {
            CANTINA_TRY_RETHROW({
            _pantoufle->setController(pan::MidiDamper::make(getNumberHarmonics(), channel, controllerIds.at(0)));
                                })
        }
        else if (type == CONTROLLER_TYPE_PAN)
        {
            CANTINA_TRY_RETHROW({
            _pantoufle->setController(pan::MidiPan::make(getNumberHarmonics(), channel, controllerIds.at(0)));
                                })
        }
        else
        {
            throw CANTINA_EXCEPTION("Controller type not recognised: " + type);
        }
    }
}
