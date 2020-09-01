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
    public:
        /** -- methods -- **/
        virtual void update(const sample_m *in, size_m blockSize) = 0;

        CANT_NODISCARD virtual float_m getPitchFreq() const = 0;
        CANT_NODISCARD virtual float_m getFidelity() const = 0;

        CANT_NODISCARD virtual bool isPitchAcceptable() const = 0;
    protected:
        /** -- fields -- **/
        // constants
        static CANT_CONSTEXPR float_m c_defaultMinFidelity = 0.9;
    };
}

#include <cant/common/undef_macro.hpp>
#endif //CANTINA_PITCHTRACKER_HPP
