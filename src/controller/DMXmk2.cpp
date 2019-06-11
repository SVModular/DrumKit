#include "DMXmk2.hpp"
#include "../samples/dmx.h"

DMXmk2Module::DMXmk2Module() : VDrumModule() {
  setupSamples();
}

void DMXmk2Module::setupSamples( ) {
  samples[0] = { (float *)dmx1, dmx1_len };
  samples[1] = { (float *)dmx2, dmx2_len };
  samples[2] = { (float *)dmx3, dmx3_len };
  samples[3] = { (float *)dmx4, dmx4_len };
  samples[4] = { (float *)dmx5, dmx5_len };
  samples[5] = { (float *)dmx6, dmx6_len };
  samples[6] = { (float *)dmx7, dmx7_len };
  samples[7] = { (float *)dmx8, dmx8_len };
  samples[8] = { (float *)dmx9, dmx9_len };
  samples[9] = { (float *)dmx10, dmx10_len };
  samples[10] = { (float *)dmx11, dmx11_len };
  samples[11] = { (float *)dmx12, dmx12_len };

  numSamples = 12;

  configParam(DMXmk2Module::TUNE_PARAM, 0.6, 1.4, 1.0);
}

struct VDrumContainer *DMXmk2Module::getSample(float current) {
  for (int i = 0; i < 12; i++) {
    if ((notes[i] - 0.02) <= current && (notes[i] + 0.02) >= current) {
      return &samples[i];
    }
  }

  return nullptr;
}
