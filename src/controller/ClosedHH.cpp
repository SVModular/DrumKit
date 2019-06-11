#include "ClosedHH.hpp"
#include "../samples/closedhh.h"

ClosedHHModule::ClosedHHModule() {
  config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
  configParam(ClosedHHModule::DRUM1_PARAM, 1.0, 15.0, 8.0);
  configParam(ClosedHHModule::DRUM2_PARAM, 1.0, 15.0, 8.0);
}

void ClosedHHModule::setupSamples( ) {
  samples[0] = { (float *)closedhh1, closedhh1_len };
  samples[1] = { (float *)closedhh2, closedhh2_len };
  samples[2] = { (float *)closedhh3, closedhh3_len };
  samples[3] = { (float *)closedhh4, closedhh4_len };
  samples[4] = { (float *)closedhh5, closedhh5_len };
  samples[5] = { (float *)closedhh6, closedhh6_len };
  samples[6] = { (float *)closedhh7, closedhh7_len };
  samples[7] = { (float *)closedhh8, closedhh8_len };
  samples[8] = { (float *)closedhh9, closedhh9_len };
  samples[9] = { (float *)closedhh10, closedhh10_len };
  samples[10] = { (float *)closedhh11, closedhh11_len };
  samples[11] = { (float *)closedhh12, closedhh12_len };
  samples[12] = { (float *)closedhh13, closedhh13_len };
  samples[13] = { (float *)closedhh14, closedhh14_len };
  samples[14] = { (float *)closedhh15, closedhh15_len };

  numSamples = 15;
}
