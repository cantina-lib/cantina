//
// Created by binabik on 29/05/2020.
//

#ifndef CANTINA_HELMHOLTZTRACKER_HPP
#define CANTINA_HELMHOLTZTRACKER_HPP

#pragma once


#include <cant/extern/helmholtz/Helmholtz.hpp>

#include <cant/common/types.hpp>
#include <cant/track/PitchTracker.hpp>

#include <cant/common/macro.hpp>
namespace cant::track
{

    class HelmholtzTracker: public PitchTracker
    {
    public:
        /** -- methods -- **/
        CANT_EXPLICIT HelmholtzTracker(
                const size_u sampleRate,
                const size_u frameSize = c_defaultFrameSize,
                const type_i overlap = c_defaultOverlap,
                const type_d minFidelity = PitchTracker::c_defaultMinFidelity
        );

        void update(const sample_f *in, size_u blockSize) override;

        CANT_NODISCARD type_d getPitchFreq() const override;
        CANT_NODISCARD type_d getFidelity() const override;
        CANT_NODISCARD bool isPitchAcceptable() const override;
    private:
        /** -- methods -- **/
        CANT_NODISCARD bool isPitchAcceptable(type_d fidelity) const;

        /** -- fields **/
        helmholtz::Helmholtz m_helm;
        size_u m_sampleRate;
        type_d m_minFidelity;
        type_d m_pitch;
        type_d m_fidelity;

        // constants
        static CANT_CONSTEXPR type_i c_defaultFrameSize = 1024;
        static CANT_CONSTEXPR type_i c_defaultOverlap = 2;
    };
}

#include <cant/common/undef_macro.hpp>
#endif //CANTINA_HELMHOLTZTRACKER_HPP
