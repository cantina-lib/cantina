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
    Cantina(const size_u numberHarmonics,
            const size_u sampleRate,
            const pan::id_u8 channelId)
    : m_pantoufle(std::make_unique<pan::Pantoufle>(numberHarmonics, channelId)),
      m_tracker(UPtr<track::PitchTracker>(new track::HelmholtzTracker(sampleRate))),
      m_shifter(UPtr<shift::TimeDomainPitchShifter>(new shift::SoundTouchShifter(numberHarmonics, sampleRate)))
    {

    }

    void
    Cantina::
    perform(const sample_f *in, sample_f *outSeed, sample_f **outHarmonics, const size_u blockSize)
    {
        /* UPDATING CANT */
        CANTINA_TRY_RETHROW({
                update(in, blockSize);
            })
        /* seed */
        std::copy(in, in + blockSize, outSeed);
        /* getting current pitch */
        if(!m_tracker->isPitchAcceptable())
        {
            /* do not do anything, return. */
            return;
        }
        type_d pitch = m_tracker->getPitchFreq();
        /* getting stream of processed notes */
        const Stream<pan::MidiNoteOutput>& processedNoteOutput = m_pantoufle->getProcessedOutputData();
        for(size_u i=0; i < getNumberHarmonics(); ++i)
        {
            const auto& note = processedNoteOutput.at(i);
            sample_f* outHarmonic = outHarmonics[i];
            CANTINA_TRY_RETHROW({
                m_shifter->apply(pitch, note, in, outHarmonic, blockSize);
            })
        }
    }

    void
    Cantina::
    update(const sample_f* in, const size_u blockSize)
    {

        CANTINA_TRY_RETHROW({
            m_tracker->update(in, blockSize);
            m_pantoufle->update();
        })
    }

    size_u
    Cantina::
    getNumberHarmonics() const
    {
        return m_pantoufle->getNumberVoices();
    }

    void
    Cantina::
    receiveNote(const pan::MidiNoteInputData& noteData)
    {
        CANTINA_TRY_RETHROW({
          m_pantoufle->receiveInputNoteData(noteData);
        })
    }

    void
    Cantina::
    receiveControl(const pan::MidiControlInputData &controlData)
    {
        CANTINA_TRY_RETHROW({
                m_pantoufle->receiveRawControlData(controlData);
        })
    }

    void
    Cantina::
    setController(const std::string &type, const pan::id_u8 channel, const Stream <pan::id_u8> &controllerIds)
    {
        if (type == CONTROLLER_TYPE_DAMPER)
        {
            CANTINA_TRY_RETHROW({
            m_pantoufle->setController(pan::MidiDamper::make(getNumberHarmonics(), channel, controllerIds.at(0)));
                                })
        }
        else if (type == CONTROLLER_TYPE_PAN)
        {
            CANTINA_TRY_RETHROW({
            m_pantoufle->setController(pan::MidiPan::make(getNumberHarmonics(), channel, controllerIds.at(0)));
                                })
        }
        else
        {
            throw CANTINA_EXCEPTION("Controller type not recognised: " + type);
        }
    }
}
