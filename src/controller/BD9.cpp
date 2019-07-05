#include "BD9.hpp"

#include <cstring>

BD9Module::BD9Module( ) {
  config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
  configParam(DRUM_PARAM, 0.0, 15.0, 7.0);
  configParam(DRUM_PARAM + 1, 0.0, 15.0, 7.0);

  configParam(TUNE_PARAM, 0.2, 1.8, 1);
  configParam(TUNE_PARAM + 1, 0.2, 1.8, 1);

  numModules = 2;

  setupSamples();
}

uint8_t BD9Module::sampleId(uint8_t which) {
  float sample = paramValue(DRUM_PARAM + which, DRUM_CV + which, 0, 15);
  if (sample >= numSamples || sample < 0) {
    return 0;
  }

  return (uint8_t) sample;
}

void BD9Module::setupSamples() {
  numSamples = 16;
  char name[32];

  for (uint8_t i = 0; i < numSamples; i++) {
    sprintf(name, "bd9-%02d", i + 1);
    samples[i] = sampleManager->selectSample(name);
  }
}
