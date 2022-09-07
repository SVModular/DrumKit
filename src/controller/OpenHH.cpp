#include "OpenHH.hpp"

OpenHHModule::OpenHHModule( ) {
  // extend the NUM_PARAMS by 1 for choke and NUM_INPUTS by 2 for choke gate and CV
  config(NUM_PARAMS + 2, NUM_INPUTS + 4, NUM_OUTPUTS, NUM_LIGHTS);
  configParam(DRUM_PARAM, 0.0, 13.0, 7.0, "Sample");
  configParam(DRUM_PARAM + 1, 0.0, 13.0, 7.0, "Sample");

  configParam(TUNE_PARAM, 0.2, 1.8, 1, "Playback Speed", "x");
  configParam(TUNE_PARAM + 1, 0.2, 1.8, 1, "Playback Speed", "x");

  // choke param
  configParam(CHOKE_PARAM, 0.05, 0.5, 0.255, "Time");
  configParam(CHOKE_PARAM + 1, 0.05, 0.5, 0.255, "Time");

  numModules = 2;

  setupSamples();

  // setup envelope parameters
  envelope[0].setAttackTime(1);
  envelope[0].setDecayTime(1);
  envelope[0].setSustainTime(1);
  envelope[0].setSustainLevel(1);
  envelope[1].setAttackTime(1);
  envelope[1].setDecayTime(1);
  envelope[1].setSustainTime(1);
  envelope[1].setSustainLevel(1);

  chokeCV[0] = new SynthDevKit::CV(0.5);
  chokeCV[1] = new SynthDevKit::CV(0.5);
}

OpenHHModule::~OpenHHModule() {
  delete chokeCV[0];
  delete chokeCV[1];
}

float OpenHHModule::chokeValue(uint8_t which, uint32_t sampleRate) {
  if (choking[which] == false) {
    return 1;
  }

  float sustainTime = paramValue(CHOKE_PARAM + which, CHOKE_CV + which, 0.01, 0.5);

  envelope[which].setReleaseTime(sampleRate * sustainTime);

  return envelope[which].step();
}

uint8_t OpenHHModule::sampleId(uint8_t which) {
  float sample = paramValue(DRUM_PARAM + which, DRUM_CV + which, 0, 14);
  if (sample >= numSamples || sample < 0) {
    return 0;
  }

  return (uint8_t) sample;
}

void OpenHHModule::setupSamples() {
  numSamples = 14;
  char name[32];

  for (uint8_t i = 0; i < numSamples; i++) {
    sprintf(name, "openhh-%02d", i + 1);
    samples[i] = sampleManager->selectSample(name);
  }
}

void OpenHHModule::process(const ProcessArgs &args) {
  for (uint8_t i = 0; i < numModules; i++) {
    if (inputs[GATE_INPUT + i].isConnected()) {
      float input = inputs[GATE_INPUT + i].getVoltage();

      cv[i]->update(input);
      if (cv[i]->newTrigger()) {
        ctx[i].reset();
        choking[i] = false;
      }
    }

    if (inputs[CHOKE_GATE + i].isConnected()) {
      float input = inputs[CHOKE_GATE + i].getVoltage();

      chokeCV[i]->update(input);
      if (chokeCV[i]->newTrigger()) {
        choking[i] = true;
        envelope[i].trigger();
      }
    }

    if (outputs[AUDIO_OUTPUT + i].isConnected()) {
      uint8_t which = sampleId(i);
      ctx[i].sample = getSample(which);
      float tune = clamp(params[TUNE_PARAM + i].getValue() + (inputs[TUNE_CV + i].isConnected() ? inputs[TUNE_CV + i].getVoltage() : 0.0f), 0.2, 1.8);
      float output = sampleManager->step(&ctx[i], tune, args.sampleRate);
      outputs[AUDIO_OUTPUT + i].setVoltage(output * chokeValue(i, args.sampleRate));
    }
  }
}
