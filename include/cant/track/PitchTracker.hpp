//
// Created by piptouque on 23/04/2020.
//

#ifndef CANTINA_PITCHTRACKER_HPP
#define CANTINA_PITCHTRACKER_HPP

#pragma once

#include <cant/common/types.hpp>
#include <cant/pan/note/MidiNoteData.hpp>
#include <cant/track/Pitch.hpp>

#include <cant/common/macro.hpp>
CANTINA_CANT_NAMESPACE_BEGIN

class PitchTracker {
public:
  /** -- methods -- **/
  CANT_EXPLICIT PitchTracker(type_d m_minConfidence);
  virtual ~PitchTracker() = default;
  virtual void update(const sample_f *in, size_u blockSize) = 0;

  CANT_NODISCARD Pitch getPitch() const;
  CANT_NODISCARD Pitch getLastValidPitch() const;
  CANT_NODISCARD bool isPitchAcceptable() const;
  CANT_NODISCARD bool isOtherPitchAcceptable(Pitch const &pitch) const;

protected:
  /** -- fields -- **/
  void setPitch(Pitch const &pitch);
  // constants
  static CANT_CONSTEXPR type_d c_defaultMinConfidence = 0.9;

private:
  /** -- methods -- **/
  /** -- fields -- **/
  type_d m_minConfidence;

  Pitch m_pitch;
  Pitch m_lastValidPitch;
};

CANTINA_CANT_NAMESPACE_END
#include <cant/common/undef_macro.hpp>

#include <cant/track/PitchTracker.inl>

#endif // CANTINA_PITCHTRACKER_HPP
