//
// Created by binabik on 29/05/2020.
//

#include <cant/track/HelmholtzTracker.hpp>

#include <cant/common/macro.hpp>
CANTINA_CANT_NAMESPACE_BEGIN

HelmholtzTracker::HelmholtzTracker(const type_i sampleRate,
                                   const size_u frameSize, const type_i overlap,
                                   const type_d minConfidence)
    : PitchTracker(minConfidence), m_helm(frameSize, overlap),
      m_sampleRate(sampleRate) {}

void HelmholtzTracker::update(const sample_f *in, const size_u blockSize) {
  m_helm.inSamples(in, blockSize);
  auto const pitch =
      Pitch(static_cast<type_d>(static_cast<cant::type_i>(m_sampleRate) /
                                m_helm.getPeriod()),
            m_helm.getFidelity());
  setPitch(pitch);
}

CANTINA_CANT_NAMESPACE_END
