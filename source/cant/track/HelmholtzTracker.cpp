//
// Created by binabik on 29/05/2020.
//

#include <cant/track/HelmholtzTracker.hpp>

namespace cant::track
{
    HelmholtzTracker::
    HelmholtzTracker(const size_u sampleRate, const size_u frameSize, const type_i overlap, const type_d minFidelity)
        : m_helm(frameSize, overlap),
        m_sampleRate(sampleRate),
        m_minFidelity(minFidelity),
        m_pitch(),
        m_fidelity()
    {}

    void
    HelmholtzTracker::
    update(const sample_f *in, const size_u blockSize)
    {
        m_helm.inSamples(in, static_cast<int>(blockSize));
        const type_d updatedFidelity = m_helm.getFidelity();
        if(isPitchAcceptable(updatedFidelity))
        {
            m_pitch = static_cast<type_d>(static_cast<cant::type_i>(m_sampleRate) / m_helm.getPeriod());
            m_fidelity = updatedFidelity;
        }
    }
}

