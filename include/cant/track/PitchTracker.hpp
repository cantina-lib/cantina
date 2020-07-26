//
// Created by piptouque on 23/04/2020.
//

#ifndef CANTINA_PITCHTRACKER_HPP
#define CANTINA_PITCHTRACKER_HPP

#pragma once


#include <cant/common/macro.hpp>

#include <cant/pan/common/types.hpp>


#include <functional>

namespace cant::track
{
    static constexpr float_m TRACKER_MIN_FIDELITY = 0.90;

    class PitchTracker
    {
    private:
    public:
        virtual void update(const sample_m *in, sizeint blockSize) = 0;

        CANT_NODISCARD virtual float_m getPitchFreq() const = 0;
        CANT_NODISCARD virtual float_m getFidelity() const = 0;

        CANT_NODISCARD virtual bool isPitchAcceptable() const = 0;
    };
}

#endif //CANTINA_PITCHTRACKER_HPP
