#include "OpenHH.hpp"
#include "../samples/openhh.h"

OpenHHModule::OpenHHModule() {
  config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
  configParam(OpenHHModule::DRUM1_PARAM, 1.0, 14.0, 7.0);
  configParam(OpenHHModule::DRUM2_PARAM, 1.0, 14.0, 7.0);
}

void OpenHHModule::setupSamples( ) {
  samples[0] = { (float *)openhh1, openhh1_len };
  samples[1] = { (float *)openhh2, openhh2_len };
  samples[2] = { (float *)openhh3, openhh3_len };
  samples[3] = { (float *)openhh4, openhh4_len };
  samples[4] = { (float *)openhh5, openhh5_len };
  samples[5] = { (float *)openhh6, openhh6_len };
  samples[6] = { (float *)openhh7, openhh7_len };
  samples[7] = { (float *)openhh8, openhh8_len };
  samples[8] = { (float *)openhh9, openhh9_len };
  samples[9] = { (float *)openhh10, openhh10_len };
  samples[10] = { (float *)openhh11, openhh11_len };
  samples[11] = { (float *)openhh12, openhh12_len };
  samples[12] = { (float *)openhh13, openhh13_len };
  samples[13] = { (float *)openhh14, openhh14_len };

  numSamples = 14;
}
