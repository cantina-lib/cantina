//
// Created by piptouque on 23/04/2020.
//

#include <cant/Cantina.hpp>
#include <cant/common/config.hpp>

#include <cant/pan/time/MidiTimer.hpp>
#include <cant/pan/note/MidiNote.hpp>
#include <cant/pan/controller/controller.hpp>
#include <cant/pan/Pantoufle.hpp>
// todo: I shouldn't have to include the headers to processors and layer classes.
// because Cantina is not in charge of constructing MidiInputPoly, MidiControllerChain and MidiEnvelopePair
// however the compiler complains that these classes are incomplete (as though it were trying to construct them?)
// (don't have a compile-time error when constructing another Pantoufle object inside the Pantoufle class
#include <cant/pan/processor/processor.hpp>
#include <cant/pan/layer/layer.hpp>

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
        // first update Pantoufle and pitch tracker.
        CANTINA_TRY_RETHROW({
            update(in, blockSize);
        })
        // seed
        std::copy(in, in + blockSize, outSeed);
        // getting current pitch
        if(!m_tracker->isPitchAcceptable())
        {
            // tracker isn't ready, return.
            return;
        }
        type_d pitch = m_tracker->getPitchFreq();
        // getting stream of processed notes
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

    CANT_NODISCARD
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
