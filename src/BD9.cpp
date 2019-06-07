#include <stdint.h>
#include "../deps/SynthDevKit/src/CV.hpp"
#include "DrumKit.hpp"
#include "DrumModule.hpp"
#include "kick.h"

#include "components.hpp"

struct BD9Module : DrumModule {
  BD9Module();
  void setupSamples( ) override;
};

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

struct BD9Widget : ModuleWidget {
    BD9Widget(BD9Module *module);
};

BD9Widget::BD9Widget(BD9Module *module) {
		setModule(module);
  box.size = Vec(3 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/BD9.svg")));

  addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
  addChild(createWidget<ScrewBlack>(
      Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

  addInput(
      createInput<CDPort>(Vec(10, 30), module, BD9Module::CLOCK1_INPUT));
  addParam(createParam<LightKnobSnap>(
      Vec(10, 72.5), module, BD9Module::DRUM1_PARAM));

  addOutput(
      createOutput<CDPort>(Vec(10, 120), module, BD9Module::AUDIO1_OUTPUT));

  addInput(
      createInput<CDPort>(Vec(10, 220), module, BD9Module::CLOCK2_INPUT));
  addParam(createParam<LightKnobSnap>(
      Vec(10, 262.5), module, BD9Module::DRUM2_PARAM));

  addOutput(
      createOutput<CDPort>(Vec(10, 310), module, BD9Module::AUDIO2_OUTPUT));
}

Model *modelBD9 = createModel<BD9Module, BD9Widget>("BassDrum9");
