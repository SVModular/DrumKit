#include <cstdint>

#include "DrumKit.hpp"
#include "VDrumModule.hpp"

#include "components.hpp"
#include "dmx.h"

struct DMXmk2Module : VDrumModule {
  float notes[12] = { 0.08, 0.17, 0.25, 0.33, 0.42, 0.5, 0.58, 0.67, 0.75, 0.83, 0.92, 1.0 };

  DMXmk2Module() : VDrumModule() {
    setupSamples();
  }

  void setupSamples( ) override {
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
  }

  struct VDrumContainer *getSample(float current) override {
    for (int i = 0; i < 12; i++) {
      if ((notes[i] - 0.02) <= current && (notes[i] + 0.02) >= current) {
        return &samples[i];
      }
    }

    return nullptr;
  }
};


struct DMXmk2Widget : ModuleWidget {
  DMXmk2Widget(DMXmk2Module *module);
};

DMXmk2Widget::DMXmk2Widget(DMXmk2Module *module) : ModuleWidget(module) {
  box.size = Vec(3 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  {
    SVGPanel *panel = new SVGPanel( );
    panel->box.size = box.size;
    panel->setBackground(SVG::load(assetPlugin(plugin, "res/DMXmk2.svg")));
    addChild(panel);
  }

  addChild(Widget::create<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
  addChild(Widget::create<ScrewBlack>(
      Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

  addInput(Port::create<CDPort>(Vec(10, 62.6), Port::INPUT, module,
                                   DMXmk2Module::GATE_INPUT));

  addInput(Port::create<CDPort>(Vec(10, 122.5), Port::INPUT, module,
                                   DMXmk2Module::VOCT_INPUT));

  addParam(ParamWidget::create<LightKnobFull>(
      Vec(10, 177.5), module, DMXmk2Module::TUNE_PARAM, 0.6, 1.4, 1.0));
  addInput(Port::create<CDPort>(Vec(10, 242.5), Port::INPUT, module,
                                   DMXmk2Module::TUNE_CV_INPUT));

  addOutput(Port::create<CDPort>(Vec(10, 302.5), Port::OUTPUT, module,
                                     DMXmk2Module::AUDIO_OUTPUT));

}

Model *modelDMXmk2 = Model::create<DMXmk2Module, DMXmk2Widget>("DrumKit", "DMX mk2", "DMX mk2", DRUM_TAG);
