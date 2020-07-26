//
// Created by binabik on 29/05/2020.
//

#ifndef CANTINA_HELMHOLTZTRACKER_HPP
#define CANTINA_HELMHOLTZTRACKER_HPP

#pragma once


#include <Helmholtz.h>

#include <cant/common/types.hpp>
#include <cant/track/PitchTracker.hpp>

namespace cant::track
{
    static constexpr float_m DEFAULT_MIN_FIDELITY = 0.9;

    class HelmholtzTracker: public PitchTracker
    {
    private:
        static constexpr int_m m_DEFAULT_FRAME_SIZE = 1024;
        static constexpr int_m m_DEFAULT_OVERLAP = 2;
    private:
        helmholtz::Helmholtz _helm;
        int_m _sampleRate;
        float_m _minFidelity;
        float_m _pitch;
        float_m _fidelity;


        CANT_NODISCARD bool isPitchAcceptable(float_m fidelity) const;
    public:
        HelmholtzTracker(
                int_m sampleRate,
                int_m frameSize = m_DEFAULT_FRAME_SIZE,
                int_m overlap = m_DEFAULT_OVERLAP,
                float_m minFidelity = DEFAULT_MIN_FIDELITY
                        );

        void update(const sample_m *in, sizeint blockSize) override;

        CANT_NODISCARD float_m getPitchFreq() const override;
        CANT_NODISCARD float_m getFidelity() const override;
        CANT_NODISCARD bool isPitchAcceptable() const override;
    };
}

#endif //CANTINA_HELMHOLTZTRACKER_HPP
