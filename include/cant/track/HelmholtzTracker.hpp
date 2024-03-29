//
// Created by binabik on 29/05/2020.
//

#ifndef CANTINA_HELMHOLTZTRACKER_HPP
#define CANTINA_HELMHOLTZTRACKER_HPP

#pragma once

#include <cant/extern/helmholtz/Helmholtz.hpp>

#include <cant/common/types.hpp>
#include <cant/track/Pitch.hpp>
#include <cant/track/PitchTracker.hpp>

#include <cant/common/macro.hpp>
CANTINA_CANT_NAMESPACE_BEGIN

class HelmholtzTracker : public PitchTracker {
public:
  /** -- methods -- **/
  CANT_EXPLICIT
  HelmholtzTracker(type_i sampleRate, size_u frameSize = c_defaultFrameSize,
                   type_i overlap = c_defaultOverlap,
                   type_d minFidelity = PitchTracker::c_defaultMinConfidence);

  void update(const sample_f *in, size_u blockSize) override;

private:
  /** -- methods -- **/
  /** -- fields **/
  Helmholtz m_helm;
  type_i m_sampleRate;

  // constants
  static CANT_CONSTEXPR size_u c_defaultFrameSize = 1024;
  static CANT_CONSTEXPR type_i c_defaultOverlap = 2;
};

CANTINA_CANT_NAMESPACE_END
#include <cant/common/undef_macro.hpp>

#endif // CANTINA_HELMHOLTZTRACKER_HPP
