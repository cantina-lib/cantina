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
                int_m sampleRate,
                int_m frameSize = c_defaultFrameSize,
                int_m overlap = c_defaultOverlap,
                float_m minFidelity = PitchTracker::c_defaultMinFidelity
        );

        void update(const sample_m *in, size_m blockSize) override;

        CANT_NODISCARD float_m getPitchFreq() const override;
        CANT_NODISCARD float_m getFidelity() const override;
        CANT_NODISCARD bool isPitchAcceptable() const override;
    private:
        /** -- methods -- **/
        CANT_NODISCARD bool isPitchAcceptable(float_m fidelity) const;

        /** -- fields **/
        helmholtz::Helmholtz _helm;
        int_m _sampleRate;
        float_m _minFidelity;
        float_m _pitch;
        float_m _fidelity;

        // constants
        static CANT_CONSTEXPR int_m c_defaultFrameSize = 1024;
        static CANT_CONSTEXPR int_m c_defaultOverlap = 2;
    };
}

#include <cant/common/undef_macro.hpp>
#endif //CANTINA_HELMHOLTZTRACKER_HPP
