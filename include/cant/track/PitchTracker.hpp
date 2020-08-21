//
// Created by piptouque on 23/04/2020.
//

#ifndef CANTINA_PITCHTRACKER_HPP
#define CANTINA_PITCHTRACKER_HPP

#pragma once

#include <cant/common/types.hpp>
#include <cant/pan/common/types.hpp>


#include <cant/common/macro.hpp>
namespace cant::track
{
    class PitchTracker
    {
    protected:
        static constexpr float_m m_DEFAULT_MIN_FIDELITY = 0.9;
    public:
        virtual void update(const sample_m *in, size_m blockSize) = 0;

        CANT_NODISCARD virtual float_m getPitchFreq() const = 0;
        CANT_NODISCARD virtual float_m getFidelity() const = 0;

        CANT_NODISCARD virtual bool isPitchAcceptable() const = 0;
    };
}

#include <cant/common/undef_macro.hpp>
#endif //CANTINA_PITCHTRACKER_HPP
