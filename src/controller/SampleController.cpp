#include "SampleController.hpp"

SampleController::SampleController( ) {
  for (uint8_t i = 0; i < MAX_MODULES; i++) {
    cv[i] = new SynthDevKit::CV(0.5f);
  }

  numSamples = 0;
  numModules = 0;
}

SampleController::~SampleController() {
  for (uint8_t i = 0; i < MAX_MODULES; i++)
    delete cv[i];
}

float SampleController::paramValue (uint16_t param, uint16_t input, float low, float high) {
  float current = params[param].getValue();

  if (inputs[input].isConnected()) {
    // high - low, divided by one tenth input voltage, plus the current value
    current += ((inputs[input].getVoltage() / 10) * (high - low));
  }

  return clamp(current, low, high);
}

DrumKit::Sample *SampleController::getSample(uint8_t current) {
  if (numSamples == 0) {
    return nullptr;
  }
  if (current < 1 || current >= numSamples) {
    return samples[ 0 ];
  }
  return samples[ current - 1 ];
}

void SampleController::process(const ProcessArgs &args) {
  for (uint8_t i = 0; i < numModules; i++) {
    if (inputs[GATE_INPUT + i].isConnected()) {
      float input = inputs[GATE_INPUT + i].getVoltage();

      cv[i]->update(input);
      if (cv[i]->newTrigger()) {
        ctx[i].reset();
      }
    }

    if (outputs[AUDIO_OUTPUT + i].isConnected()) {
      uint8_t which = sampleId(i);
      ctx[i].sample = getSample(which);
      float tune = clamp(params[TUNE_PARAM + i].getValue() + (inputs[TUNE_CV + i].isConnected() ? inputs[TUNE_CV + i].getVoltage() : 0.0f), 0.2, 1.8);
      float output = sampleManager->step(&ctx[i], tune, args.sampleRate);
      outputs[AUDIO_OUTPUT + i].setVoltage(output);
    }
  }
}

void SampleController::setupSamples( ) { }
uint8_t SampleController::sampleId(uint8_t which) { return 0; }
float SampleController::getTune(uint8_t which) { return 1.0f; }
