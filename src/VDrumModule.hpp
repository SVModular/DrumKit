#pragma once

#include <cstdint>
#include <cmath>

#include "../deps/SynthDevKit/src/CV.hpp"

#include "DrumKit.hpp"

struct VDrumContainer {
  float *sample;
  unsigned int length;
};

struct VDrumModule : Module {
  enum ParamIds { ATTENUATE_PARAM, TUNE_PARAM, NUM_PARAMS };
  enum InputIds { GATE_INPUT, VOCT_INPUT, TUNE_CV_INPUT, NUM_INPUTS };
  enum OutputIds { AUDIO_OUTPUT, NUM_OUTPUTS };
  enum LightIds { NUM_LIGHTS };

  VDrumModule( );
  void process(const ProcessArgs &) override;
  virtual void setupSamples( );
  virtual struct VDrumContainer *getSample(float);
  float getSampleValue(double, float);
  SynthDevKit::CV *cv;
  double currentStep;
  uint8_t currentDrum;
  struct VDrumContainer samples[32];
  uint8_t numSamples;
  bool playing = false;
};
