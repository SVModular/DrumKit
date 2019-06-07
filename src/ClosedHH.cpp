#include <stdint.h>

#include "../deps/SynthDevKit/src/CV.hpp"
#include "DrumKit.hpp"
#include "DrumModule.hpp"
#include "closedhh.h"
#include "components.hpp"

struct ClosedHHModule : DrumModule {
  ClosedHHModule();
  void setupSamples( ) override;
};

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

struct ClosedHHWidget : ModuleWidget {
    ClosedHHWidget(ClosedHHModule *module);
};

ClosedHHWidget::ClosedHHWidget(ClosedHHModule *module) {
		setModule(module);
  box.size = Vec(3 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/ClosedHH.svg")));


  addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
  addChild(createWidget<ScrewBlack>(
      Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

  addInput(createInput<CDPort>(Vec(10, 30), module,
                                   ClosedHHModule::CLOCK1_INPUT));
  addParam(createParam<LightKnobSnap>(
      Vec(10, 72.5), module, ClosedHHModule::DRUM1_PARAM));

  addOutput(createOutput<CDPort>(Vec(10, 120), module,
                                     ClosedHHModule::AUDIO1_OUTPUT));

  addInput(createInput<CDPort>(Vec(10, 220), module,
                                   ClosedHHModule::CLOCK2_INPUT));
  addParam(createParam<LightKnobSnap>(
      Vec(10, 262.5), module, ClosedHHModule::DRUM2_PARAM));

  addOutput(createOutput<CDPort>(Vec(10, 310), module,
                                     ClosedHHModule::AUDIO2_OUTPUT));
}

Model *modelClosedHH = createModel<ClosedHHModule, ClosedHHWidget>("ClosedHiHat");
