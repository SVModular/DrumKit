#pragma once

#include "SynthDevKit/src/CV.hpp"
#include <cmath>

class LowFrequencyOscillator {
private:
  float phase;
  float pw;
  float freq;
  float shift;
  float shiftVal;
  float random;
  bool skip;
  bool offset;
  bool invert;
  SynthDevKit::CV *cv;

public:
  LowFrequencyOscillator();
  ~LowFrequencyOscillator();
  void setPitch(float);
  void setFrequency(float);
  void setPulseWidth(float);
  void setShift(float);
  void setRandom(float);
  void setInvert(bool);
  void setReset(float);
  void hardReset();
  void step(float);
  float sin();
  float tri(float);
  float tri();
  float saw(float);
  float saw();
  float sqr();
  float progress();
};
