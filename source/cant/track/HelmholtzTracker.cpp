//
// Created by binabik on 29/05/2020.
//

#include <cant/track/HelmholtzTracker.hpp>

namespace cant::track
{
    HelmholtzTracker::
    HelmholtzTracker(const type_i sampleRate, const type_i frameSize, const type_i overlap, const type_d minFidelity)
    : _helm(frameSize, overlap), _sampleRate(sampleRate), _minFidelity(minFidelity), _pitch(), _fidelity()
    {

    }

    void
    HelmholtzTracker::
    update(const sample_f *in, const size_u blockSize)
    {
        _helm.inSamples(in, static_cast<int>(blockSize));
        const type_d updatedFidelity = _helm.getFidelity();
        if(isPitchAcceptable(updatedFidelity))
        {
            _pitch = static_cast<type_d>(_sampleRate / _helm.getPeriod());
            _fidelity = updatedFidelity;
        }
    }

    type_d
    HelmholtzTracker::
    getPitchFreq() const
    {
        return _pitch;
    }

    type_d
    HelmholtzTracker::
    getFidelity() const
    {
        return _fidelity;
    }

    bool
    HelmholtzTracker::
    isPitchAcceptable(const type_d fidelity) const
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

