#include <stdint.h>

#include "../deps/SynthDevKit/src/CV.hpp"
#include "DrumKit.hpp"
#include "DrumModule.hpp"
#include "snare.h"

struct SnareModule : DrumModule {
  void setupSamples( ) override;
};

void SnareModule::setupSamples( ) {
  samples[0] = { (float *)snare1, snare1_len };
  samples[1] = { (float *)snare2, snare2_len };
  samples[2] = { (float *)snare3, snare3_len };
  samples[3] = { (float *)snare4, snare4_len };
  samples[4] = { (float *)snare5, snare5_len };
  samples[5] = { (float *)snare6, snare6_len };
  samples[6] = { (float *)snare7, snare7_len };
  samples[7] = { (float *)snare8, snare8_len };
  samples[8] = { (float *)snare9, snare9_len };
  samples[9] = { (float *)snare10, snare10_len };
  samples[10] = { (float *)snare11, snare11_len };
  samples[11] = { (float *)snare12, snare12_len };
  samples[12] = { (float *)snare13, snare13_len };
  samples[13] = { (float *)snare14, snare14_len };
  samples[14] = { (float *)snare15, snare15_len };
  samples[15] = { (float *)snare16, snare16_len };

  numSamples = 16;
}

SnareWidget::SnareWidget( ) {
  SnareModule *module = new SnareModule( );
  setModule(module);
  box.size = Vec(3 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  {
    SVGPanel *panel = new SVGPanel( );
    panel->box.size = box.size;
    panel->setBackground(SVG::load(assetPlugin(plugin, "res/Snare.svg")));
    addChild(panel);
  }

  addChild(createScrew<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
  addChild(createScrew<ScrewSilver>(
      Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

  addInput(
      createInput<PJ301MPort>(Vec(10, 45), module, SnareModule::CLOCK1_INPUT));
  addParam(createParam<RoundSmallBlackSnapKnob>(
      Vec(8, 92), module, SnareModule::DRUM1_PARAM, 1.0, 16.0, 8.0));

  addOutput(createOutput<PJ301MPort>(Vec(10, 149), module,
                                     SnareModule::AUDIO1_OUTPUT));

  addInput(
      createInput<PJ301MPort>(Vec(10, 205), module, SnareModule::CLOCK2_INPUT));
  addParam(createParam<RoundSmallBlackSnapKnob>(
      Vec(8, 252), module, SnareModule::DRUM2_PARAM, 1.0, 16.0, 8.0));

  addOutput(createOutput<PJ301MPort>(Vec(10, 308), module,
                                     SnareModule::AUDIO2_OUTPUT));
}
