//
// Created by piptouque on 23/04/2020.
//

#include <cant/Cantina.hpp>

#include <cant/common/config.hpp>

#include <cant/pan/controller/controller.hpp>

#include <cant/common/CantinaException.hpp>

#include <cant/common/macro.hpp>
CANTINA_CANT_NAMESPACE_BEGIN

Cantina::Cantina(const size_u numberHarmonics, const type_i sampleRate,
                 const pan::id_u8 channelId)
    : m_pantoufle(std::make_unique<pan::Pantoufle>(numberHarmonics, channelId)),
      m_tracker(static_cast<UPtr<PitchTracker>>(
          std::make_unique<HelmholtzTracker>(sampleRate))),
      m_shifter(static_cast<UPtr<TimeDomainPitchShifter>>(
          std::make_unique<SoundTouchShifter>(numberHarmonics, sampleRate))) {}

void Cantina::perform(const sample_f *in, sample_f *outSeed,
                      sample_f **outHarmonics, const size_u blockSize) {
  // first updateDelta pitch tracker.
  CANTINA_TRY_RETHROW({ m_tracker->update(in, blockSize); })
  // seed
  std::copy(in, in + blockSize, outSeed);
  // getting current pitch
  if (!m_tracker->isPitchAcceptable()) {
    // tracker isn't ready, return.
    return;
  }
  type_d pitch = m_tracker->getPitchFreq();
  // getting stream of processed notes
  const auto &processedNoteOutput = m_pantoufle->getProcessedNoteOutput();
  for (size_u i = 0; i < getNumberHarmonics(); ++i) {
    const auto &note = processedNoteOutput.at(i);
    if (note.isSet()) {
      sample_f *outHarmonic = outHarmonics[i];
      CANTINA_TRY_RETHROW(
          { m_shifter->apply(pitch, note, in, outHarmonic, blockSize); })
    }
  }
}

void Cantina::setCustomClock(time::AbsoluteTimeGetter currentTimeGetter) {
  m_pantoufle->setCustomClock(std::move(currentTimeGetter));
}

void Cantina::update(){CANTINA_TRY_RETHROW({ m_pantoufle->update(); })}

size_u Cantina::getNumberHarmonics() const {
  return m_pantoufle->getNumberVoices();
}

Optional<size_u> Cantina::receiveNote(const pan::MidiNoteInputData &noteData) {
  CANTINA_TRY_RETHROW({ return m_pantoufle->receiveInputNoteData(noteData); })
}

void Cantina::receiveControl(const pan::MidiControlInputData &controlData) {
  CANTINA_TRY_RETHROW({ m_pantoufle->receiveRawControlData(controlData); })
}

const pan::MidiNoteOutput &Cantina::getProcessedVoice(size_u voice) const {
  return m_pantoufle->getProcessedVoice(voice);
}
void Cantina::addEnvelope(UPtr<pan::MidiEnvelope> envelope) {
  m_pantoufle->addEnvelope(std::move(envelope));
}

CANTINA_CANT_NAMESPACE_END
