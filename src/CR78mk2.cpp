#include <cstdint>

#include "DrumKit.hpp"
#include "VDrumModule.hpp"

#include "components.hpp"
#include "cr78.h"

struct CR78mk2Module : VDrumModule {
  float notes[12] = { 0.08, 0.17, 0.25, 0.33, 0.42, 0.5, 0.58, 0.67, 0.75, 0.83, 0.92, 1.0 };

  CR78mk2Module() : VDrumModule() {
    setupSamples();
    configParam(CR78mk2Module::TUNE_PARAM, 0.6, 1.4, 1.0);
  }

  void setupSamples( ) override {
    samples[0] = { (float *)cr781, cr781_len };
    samples[1] = { (float *)cr782, cr782_len };
    samples[2] = { (float *)cr783, cr783_len };
    samples[3] = { (float *)cr784, cr784_len };
    samples[4] = { (float *)cr785, cr785_len };
    samples[5] = { (float *)cr786, cr786_len };
    samples[6] = { (float *)cr787, cr787_len };

    numSamples = 7;
  }

  struct VDrumContainer *getSample(float current) override {
    for (int i = 0; i < 7; i++) {
      if ((notes[i] - 0.02) <= current && (notes[i] + 0.02) >= current) {
        return &samples[i];
      }
    }

    return nullptr;
  }
};


struct CR78mk2Widget : ModuleWidget {
  CR78mk2Widget(CR78mk2Module *module);
};

CR78mk2Widget::CR78mk2Widget(CR78mk2Module *module) {
		setModule(module);
  box.size = Vec(3 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/CR78mk2.svg")));

  addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
  addChild(createWidget<ScrewBlack>(
      Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

  addInput(createInput<CDPort>(Vec(10, 62.6), module,
                                   CR78mk2Module::VOCT_INPUT));

  addInput(createInput<CDPort>(Vec(10, 122.5), module,
                                   CR78mk2Module::GATE_INPUT));

  addParam(createParam<LightKnobFull>(
      Vec(10, 177.5), module, CR78mk2Module::TUNE_PARAM));
  addInput(createInput<CDPort>(Vec(10, 242.5), module,
                                   CR78mk2Module::TUNE_CV_INPUT));

  addOutput(createOutput<CDPort>(Vec(10, 302.5), module,
                                     CR78mk2Module::AUDIO_OUTPUT));

}

Model *modelCR78mk2 = createModel<CR78mk2Module, CR78mk2Widget>("CR78mk2");
