//
// Created by binabik on 28/08/2020.
//

#ifndef CANTINA_TILDE_FFTWPERFORMER_INL
#define CANTINA_TILDE_FFTWPERFORMER_INL

#pragma once

#include <cant/common/macro.hpp>
namespace cant::fft
{
    CANT_NODISCARD CANT_INLINE
    size_u
    FFTWPerformer::
    getTransformSize() const
    { return m_inoutBuffer.size(); }

}

#include <cant/common/undef_macro.hpp>
#endif //CANTINA_TILDE_FFTWPERFORMER_INL
