#ifndef CANTINA_TILDE_PITCHTRACKER_INL
#define CANTINA_TILDE_PITCHRACKER_INL

#include "cant/track/PitchTracker.hpp"
#include <algorithm>
#pragma once

#include <cant/common/types.hpp>

#include <cant/common/macro.hpp>
CANTINA_CANT_NAMESPACE_BEGIN

CANT_INLINE
PitchTracker::PitchTracker(type_d minConfidence)
    : m_minConfidence(minConfidence), m_pitch(), m_lastValidPitch() {}

CANT_INLINE
bool PitchTracker::isPitchAcceptable() const {
  return isOtherPitchAcceptable(getPitch());
}

CANT_INLINE
bool PitchTracker::isOtherPitchAcceptable(Pitch const &pitch) const {
  return pitch.getConfidence() > m_minConfidence;
}

CANT_INLINE
Pitch PitchTracker::getPitch() const { return m_pitch; }

CANT_INLINE
Pitch PitchTracker::getLastValidPitch() const { return m_lastValidPitch; }

CANT_INLINE
void PitchTracker::setPitch(Pitch const &pitch) {
  if (isOtherPitchAcceptable(pitch)) {
    m_lastValidPitch = pitch;
  }
  m_pitch = pitch;
}

CANTINA_CANT_NAMESPACE_END
#include <cant/common/undef_macro.hpp>

#endif // CANTINA_TILDE_PITCHTRACKER_INL
