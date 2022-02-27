
#ifndef CANTINA_PITCH_HPP
#define CANTINA_PITCH_HPP

#pragma once

#include <cant/common/types.hpp>
#include <cant/pan/note/MidiNoteData.hpp>

#include <cant/common/macro.hpp>
CANTINA_CANT_NAMESPACE_BEGIN

class Pitch {
public:
  /** -- methods -- **/
  Pitch(type_d value, type_d confidence);
  Pitch();
  CANT_NODISCARD type_d getFreq() const;
  CANT_NODISCARD type_d getConfidence() const;

private:
  /** -- fields -- **/
  type_d m_value;
  type_d m_confidence;
};

CANTINA_CANT_NAMESPACE_END
#include <cant/common/undef_macro.hpp>

#include <cant/track/Pitch.inl>

#endif // CANTINA_PITCH_HPP
