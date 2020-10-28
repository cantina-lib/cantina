//
// Created by binabik on 28/08/2020.
//

#ifndef CANTINA_TILDE_FFTWPERFORMER_INL
#define CANTINA_TILDE_FFTWPERFORMER_INL

#pragma once

#include <cant/common/macro.hpp>
CANTINA_CANT_NAMESPACE_BEGIN

CANT_NODISCARD CANT_INLINE size_u FFTWPerformer::getTransformSize() const {
  return m_inoutBuffer.size();
}

CANTINA_CANT_NAMESPACE_END

#include <cant/common/undef_macro.hpp>
#endif // CANTINA_TILDE_FFTWPERFORMER_INL
