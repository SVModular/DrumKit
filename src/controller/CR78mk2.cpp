#include "CR78mk2.hpp"
#include "../samples/cr78.h"

CR78mk2Module::CR78mk2Module() : VDrumModule() {
  setupSamples();
  configParam(CR78mk2Module::TUNE_PARAM, 0.6, 1.4, 1.0);
}

void CR78mk2Module::setupSamples( ) {
  samples[0] = { (float *)cr781, cr781_len };
  samples[1] = { (float *)cr782, cr782_len };
  samples[2] = { (float *)cr783, cr783_len };
  samples[3] = { (float *)cr784, cr784_len };
  samples[4] = { (float *)cr785, cr785_len };
  samples[5] = { (float *)cr786, cr786_len };
  samples[6] = { (float *)cr787, cr787_len };

  numSamples = 7;
}

struct VDrumContainer *CR78mk2Module::getSample(float current) {
  for (int i = 0; i < 7; i++) {
    if ((notes[i] - 0.02) <= current && (notes[i] + 0.02) >= current) {
      return &samples[i];
    }
  }

  return nullptr;
}
