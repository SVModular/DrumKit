#include "BD9.hpp"

#include "../samples/kick.h"

BD9Module::BD9Module( ) {
  config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
  configParam(BD9Module::DRUM1_PARAM, 1.0, 16.0, 8.0);
  configParam(BD9Module::DRUM2_PARAM, 0.0, 16.0, 8.0);
}

void BD9Module::setupSamples( ) {
  samples[0] = { (float *)kick1, kick1_len };
  samples[1] = { (float *)kick2, kick2_len };
  samples[2] = { (float *)kick3, kick3_len };
  samples[3] = { (float *)kick4, kick4_len };
  samples[4] = { (float *)kick5, kick5_len };
  samples[5] = { (float *)kick6, kick6_len };
  samples[6] = { (float *)kick7, kick7_len };
  samples[7] = { (float *)kick8, kick8_len };
  samples[8] = { (float *)kick9, kick9_len };
  samples[9] = { (float *)kick10, kick10_len };
  samples[10] = { (float *)kick11, kick11_len };
  samples[11] = { (float *)kick12, kick12_len };
  samples[12] = { (float *)kick13, kick13_len };
  samples[13] = { (float *)kick14, kick14_len };
  samples[14] = { (float *)kick15, kick15_len };
  samples[15] = { (float *)kick16, kick16_len };

  numSamples = 16;
}
