#include <cstdint>
#include <cmath>

#include "../deps/SynthDevKit/src/CV.hpp"
#include "DrumKit.hpp"
#include "VDrumModule.hpp"


VDrumModule::VDrumModule( ) : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
  cv          = new SynthDevKit::CV(1.7f);
  currentStep = 0;
  numSamples  = 0;
}

void VDrumModule::setupSamples() { }


struct VDrumContainer *VDrumModule::getSample(float current) {
  if (numSamples == 0) {
    setupSamples();
  }
  if (current < 1 || current >= numSamples) {
    return &samples[ 0 ];
  }
  return &samples[ (int)current - 1 ];
}

void VDrumModule::step( ) {
  // attenuation value
  float gate = inputs[ GATE_INPUT ].value;
  cv->update(gate);

  if (cv->newTrigger( )) {
    currentStep = 0.0f;
    playing = true;
  }

  if (playing) {
    float current           = inputs[ VOCT_INPUT ].value;

    float tune = clamp(params[TUNE_PARAM].value + (inputs[TUNE_CV_INPUT].active ? inputs[TUNE_CV_INPUT].value : 0.0f), 0.2, 1.8);
    outputs[ AUDIO_OUTPUT ].value = getSampleValue(current, tune);
    currentStep += tune;
  } else {
    outputs[AUDIO_OUTPUT].value = 0.0f;
  }
}

float VDrumModule::getSampleValue (double current, float tune) {
  double rem, dden;

  rem = modf (currentStep , &dden);
  uint32_t den = (uint32_t) dden;

  struct VDrumContainer *c = getSample(current);

  if (c == nullptr) {
    playing = false;
    return 0.0f;
  }

  // if the next step is past the end, just return the last value
  // kind of a cheap way out of doing math, but that's ok
  if (currentStep + tune >= c->length) {
    playing = false;
    return 0.0f;
  }

  // interpolate the resulting
  double p = currentStep + tune;
  double trem, dtden;

  trem = modf(p, &dtden);
  uint32_t tden = (uint32_t) dtden;

  // if they're the same, time to do some math
  if (den == tden) {
    // if the next value is again past the number of samples, we can bail again
    // without doing much math
    if (den + 1 >= c->length) {
      playing = false;
      return c->sample[den];
    }
  }

  // otherwise figure out the difference between the two, and average them
  float f1 = (rem * c->sample[den]) + ((1 - rem) * c->sample[den + 1]);
  float f2 = (trem * c->sample[tden]) + ((1 - trem) * c->sample[tden + 1]);

  // iterate through the whole numbers, and add those in too
  float total = f1 + f2;
  uint32_t count = 0;

  for (uint32_t i = den + 1; i < tden; i++, count++) {
    total += c->sample[i];
  }

  // return the average
  return total / (2 + count);
}
