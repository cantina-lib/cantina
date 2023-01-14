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

void Cantina::perform(sample_f const *inSeed, sample_f const *inTrack,
                      sample_f **outHarmonics, size_u blockSize) {
  // first updateDelta pitch tracker.
  CANTINA_TRY_RETHROW({ m_tracker->update(inTrack, blockSize); })
  // getting last valid pitch
  auto const pitch = m_tracker->getLastValidPitch();
  if (!m_tracker->isOtherPitchAcceptable(pitch)) {
    // tracker isn't ready, return.
    return;
  }
  // getting stream of processed notes
  const auto &processedNoteOutput = m_pantoufle->getProcessedNoteOutput();
  for (size_u i = 0; i < getNumberVoices(); ++i) {
    const auto &note = processedNoteOutput.at(i);
    if (note.isSet()) {
      sample_f *outHarmonic = outHarmonics[i];
      CANTINA_TRY_RETHROW({
        m_shifter->apply(pitch.getFreq(), note, inSeed, outHarmonic, blockSize);
      })
    }
  }
}

void Cantina::setCustomClock(time::AbsoluteTimeGetter currentTimeGetter) {
  m_pantoufle->setCustomClock(std::move(currentTimeGetter));
}

void Cantina::update(){CANTINA_TRY_RETHROW({ m_pantoufle->update(); })}

size_u Cantina::getNumberVoices() const {
  return m_pantoufle->getNumberVoices();
}

Pitch Cantina::getLastValidPitch() const {
  return m_tracker->getLastValidPitch();
}
Pitch Cantina::getPitch() const {
  return m_tracker->getPitch();
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
