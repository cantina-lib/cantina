
#ifndef CANTINA_TILDE_PITCH_INL
#define CANTINA_TILDE_PITCH_INL

#include "cant/track/Pitch.hpp"
#pragma once

#include <cant/common/types.hpp>

#include <cant/common/macro.hpp>
CANTINA_CANT_NAMESPACE_BEGIN

CANT_INLINE
Pitch::Pitch(type_d value, type_d confidence)
    : m_value(value), m_confidence(confidence) {}

CANT_INLINE
Pitch::Pitch() : m_value(-1), m_confidence(0) {}

CANT_INLINE
type_d Pitch::getFreq() const { return m_value; }

CANT_INLINE type_d Pitch::getConfidence() const { return m_confidence; }

CANTINA_CANT_NAMESPACE_END
#include <cant/common/undef_macro.hpp>

#endif // CANTINA_TILDE_PITCH_INL
