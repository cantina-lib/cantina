//
// Created by binabik on 26/07/2020.
//

#ifndef CANTINA_FFTWANALYSIS_HPP
#define CANTINA_FFTWANALYSIS_HPP

#pragma once

#include <cant/common/memory.hpp>
#include <cant/common/types.hpp>

#include <cant/fft/FFTWAllocator.hpp>

#include <cant/common/macro.hpp>
CANTINA_CANT_NAMESPACE_BEGIN

class FFTWPerformer {
public:
  /** -- methods -- **/
  CANT_EXPLICIT FFTWPerformer(size_u transformSize);
  ~FFTWPerformer();
  void performRealForward(Stream<sample_f> &inout);
  void performRealInverse(Stream<sample_f> &inout);

  CANT_NODISCARD size_u getTransformSize() const;

private:
  /** -- methods -- **/
  void performReal(Stream<sample_f> &inout, const fftwf_plan &realPlan);

  // static methods
  static fftwf_plan computeRealPlan(FFTW3FBlock &inOutBuffer,
                                    fftw_r2r_kind kind);

  /** -- fields -- **/
  FFTW3FBlock m_inoutBuffer;
  fftwf_plan m_realForwardPlan;
  fftwf_plan m_realInversePlan;
};

CANTINA_CANT_NAMESPACE_END
#include <cant/common/undef_macro.hpp>

#include <cant/fft/FFTWPerformer.inl>

#endif // CANTINA_FFTWANALYSIS_HPP
