#include "DMX.hpp"

#include <cstring>

DMXModule::DMXModule( ) {
  config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
  configParam(DRUM_PARAM, 0.0, 11.0, 5.0, "Sample");
  configParam(DRUM_PARAM + 1, 0.0, 11.0, 5.0, "Sample");

  configParam(TUNE_PARAM, 0.2, 1.8, 1, "Playback Speed", "x");
  configParam(TUNE_PARAM + 1, 0.2, 1.8, 1, "Playback Speed", "x");

  numModules = 2;

  setupSamples();
}

uint8_t DMXModule::sampleId(uint8_t which) {
  float sample = paramValue(DRUM_PARAM + which, DRUM_CV + which, 0, 15);
  if (sample >= numSamples || sample < 0) {
    return 0;
  }

  return (uint8_t) sample;
}

void DMXModule::setupSamples() {
  numSamples = 12;
  char name[32];

  for (uint8_t i = 0; i < numSamples; i++) {
    sprintf(name, "DMX-%02d", i + 1);
    samples[i] = sampleManager->selectSample(name);
  }
}
