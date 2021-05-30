//
// Created by binabik on 03/06/2020.
//

#ifndef CANTINA_TIMEDOMAINPITCHSHIFTER_HPP
#define CANTINA_TIMEDOMAINPITCHSHIFTER_HPP

#pragma once

#include <cant/shift/PitchShifter.hpp>

#include <cant/common/macro.hpp>
CANTINA_CANT_NAMESPACE_BEGIN

class TimeDomainPitchShifter : public PitchShifter {
public:
protected:
  /** -- methods -- **/
  CANT_NODISCARD size_u getNumberSamplesAvailable(size_u voice) const override =
      0;
  CANT_NODISCARD type_i getSampleRate() const override = 0;

private:
  /** -- methods -- **/
  // private inheritance
  void update(size_u voice, const sample_f *input,
              size_u blockSize) override = 0;
  void shift(pan::tone_d src, const pan::MidiNoteOutput &note,
             const sample_f *input, sample_f *output,
             size_u blockSize) override;
  virtual void shiftVoice(size_u voice, type_d shiftRatio, const sample_f *input,
                          sample_f *output, size_u blockSize) = 0;

  void clearBuffers(size_u voice) override = 0;
  void trimBuffers(size_u voice, size_u numberSamples) override = 0;
};

CANTINA_CANT_NAMESPACE_END
#include <cant/common/undef_macro.hpp>

#endif // CANTINA_TIMEDOMAINPITCHSHIFTER_HPP
