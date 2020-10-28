//
// Created by binabik on 29/05/2020.
//

#ifndef CANTINA_RUBBERTOUCHSHIFTER_HPP
#define CANTINA_RUBBERTOUCHSHIFTER_HPP

#pragma once

#include <RubberBandStretcher.h>

#include <cant/common/types.hpp>

#include <cant/shift/MixedDomainPitchShifter.hpp>

#include <cant/common/macro.hpp>
CANTINA_CANT_NAMESPACE_BEGIN

class TimeDomainPitchShifter;

class RubberTouchShifter final : public MixedDomainPitchShifter {
public:
  /** -- methods -- **/
  RubberTouchShifter(UPtr<TimeDomainPitchShifter> auxShifter,
                     size_u numberVoices, type_i sampleRate, size_u blockSize);

protected:
  /** -- methods -- **/
  void clearBuffers(size_u voice) override;
  void update(size_u voice, const sample_f *input, size_u blockSize) override;

  CANT_NODISCARD bool isReady(size_u voice) const override;

  /** -- fields -- **/
  std::vector<RubberBand::RubberBandStretcher> m_bands;

  // constants
  static CANT_CONSTEXPR size_u c_maxProcessBatchSize = 1024;
};

CANTINA_CANT_NAMESPACE_END
#include <cant/common/undef_macro.hpp>

#endif // CANTINA_RUBBERTOUCHSHIFTER_HPP
