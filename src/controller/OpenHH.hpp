#include "SampleController.hpp"
#include "../model/Envelope.hpp"

struct OpenHHModule : SampleController {
  OpenHHModule();
  ~OpenHHModule() override;
  void setupSamples( ) override;
  uint8_t sampleId(uint8_t) override;
  float chokeValue(uint8_t, uint32_t);
  void process(const ProcessArgs &) override;

  enum ParamIds { DRUM_PARAM, TUNE_PARAM = MAX_MODULES, CHOKE_PARAM = TUNE_PARAM + MAX_MODULES, NUM_PARAMS = CHOKE_PARAM + MAX_MODULES };
  enum InputIds { DRUM_CV, GATE_INPUT = MAX_MODULES, TUNE_CV = GATE_INPUT + MAX_MODULES, CHOKE_CV = TUNE_CV + MAX_MODULES, CHOKE_GATE = CHOKE_CV + MAX_MODULES, NUM_INPUTS = CHOKE_GATE + MAX_MODULES };

  Envelope envelope[2];

  SynthDevKit::CV *chokeCV[2];
  bool choking[2];
};
