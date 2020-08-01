//
// Created by binabik on 29/05/2020.
//

#include <cant/track/HelmholtzTracker.hpp>

#include <iostream>

namespace cant::track
{
    HelmholtzTracker::
    HelmholtzTracker(const int_m sampleRate, const int_m frameSize, const int_m overlap, const float_m minFidelity)
    : _helm(frameSize, overlap), _sampleRate(sampleRate), _minFidelity(minFidelity), _pitch(), _fidelity()
    {

    }

    void
    HelmholtzTracker::
    update(const sample_m *in, const sizeint blockSize)
    {
        _helm.inSamples(in, blockSize);
        const float_m updatedFidelity = _helm.getFidelity();
        if(isPitchAcceptable(updatedFidelity))
        {
            _pitch = (float_m)_sampleRate / _helm.getPeriod();
            _fidelity = updatedFidelity;
        }
        std::cout << _pitch << " | " << _fidelity << std::endl;
    }

    float_m
    HelmholtzTracker::
    getPitchFreq() const
    {
        return _pitch;
    }

    float_m
    HelmholtzTracker::
    getFidelity() const
    {
        return _fidelity;
    }

    bool
    HelmholtzTracker::
    isPitchAcceptable(const float_m fidelity) const
    {
        return fidelity > _minFidelity;
    }

    bool
    HelmholtzTracker::
    isPitchAcceptable() const
    {
        return isPitchAcceptable(_fidelity);
    }
}

