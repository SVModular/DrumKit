#pragma once

#include <cstdint>
#include "../../deps/SynthDevKit/src/CV.hpp"
#include "../DrumKit.hpp"
#include "../model/SampleManager.hpp"

// the maximum number of drums to support in a single module
#define MAX_MODULES 16
#define MAX_SAMPLES 32

struct SampleController : Module {
  enum ParamIds { DRUM_PARAM, TUNE_PARAM = MAX_MODULES, NUM_PARAMS = TUNE_PARAM + MAX_MODULES };
  enum InputIds { DRUM_CV, GATE_INPUT = MAX_MODULES, TUNE_CV = GATE_INPUT + MAX_MODULES, NUM_INPUTS = TUNE_CV + MAX_MODULES };
  enum OutputIds { AUDIO_OUTPUT, NUM_OUTPUTS = MAX_MODULES };
  enum LightIds { NUM_LIGHTS };

  SampleController( );
  ~SampleController() override;
  void process(const ProcessArgs &) override;
  float paramValue (uint16_t, uint16_t, float, float);
  virtual void setupSamples( );
  DrumKit::Sample *getSample(uint8_t);
  virtual uint8_t sampleId(uint8_t);
  virtual float getTune(uint8_t);

  SynthDevKit::CV *cv[MAX_MODULES];
  DrumKit::SampleManagerContext ctx[MAX_MODULES];

  DrumKit::Sample *samples[MAX_SAMPLES];
  uint8_t numSamples;

  uint8_t numModules;
};
