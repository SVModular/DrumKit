#include <stdint.h>

#include "../deps/SynthDevKit/src/CV.hpp"
#include "DrumKit.hpp"
#include "DrumModule.hpp"
#include "openhh.h"

#include "components.hpp"

struct OpenHHModule : DrumModule {
  OpenHHModule();
  void setupSamples( ) override;
};

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

struct OpenHHWidget : ModuleWidget {
  OpenHHWidget(OpenHHModule *module);
};

OpenHHWidget::OpenHHWidget(OpenHHModule *module) {
		setModule(module);
  box.size = Vec(3 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/OpenHH.svg")));

  addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
  addChild(createWidget<ScrewBlack>(
      Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

  addInput(createInput<CDPort>(Vec(10, 30), module,
                                   OpenHHModule::CLOCK1_INPUT));
  addParam(createParam<LightKnobSnap>(
      Vec(10, 72.5), module, OpenHHModule::DRUM1_PARAM));

  addOutput(createOutput<CDPort>(Vec(10, 120), module,
                                     OpenHHModule::AUDIO1_OUTPUT));

  addInput(createInput<CDPort>(Vec(10, 220), module,
                                   OpenHHModule::CLOCK2_INPUT));
  addParam(createParam<LightKnobSnap>(
      Vec(10, 262.5), module, OpenHHModule::DRUM2_PARAM));

  addOutput(createOutput<CDPort>(Vec(10, 310), module,
                                     OpenHHModule::AUDIO2_OUTPUT));
}

Model *modelOpenHH = createModel<OpenHHModule, OpenHHWidget>("OpenHiHat");
