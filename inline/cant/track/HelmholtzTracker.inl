//
// Created by binabik on 13/09/2020.
//

#ifndef CANTINA_TILDE_HELMHOLTZTRACKER_INL
#define CANTINA_TILDE_HELMHOLTZTRACKER_INL

#pragma once

#include <cant/common/types.hpp>

#include <cant/common/macro.hpp>
namespace cant::track
{
    CANT_INLINE
    type_d
    HelmholtzTracker::
    getPitchFreq() const
    {
        return m_pitch;
    }

    CANT_INLINE
    type_d
    HelmholtzTracker::
    getFidelity() const
    {
        return m_fidelity;
    }

    CANT_INLINE
    bool
    HelmholtzTracker::
    isPitchAcceptable(const type_d fidelity) const
    {
        return fidelity > m_minFidelity;
    }

    CANT_INLINE
    bool
    HelmholtzTracker::
    isPitchAcceptable() const
    {
        return isPitchAcceptable(m_fidelity);
    }
}
#include <cant/common/undef_macro.hpp>

#endif //CANTINA_TILDE_HELMHOLTZTRACKER_INL
