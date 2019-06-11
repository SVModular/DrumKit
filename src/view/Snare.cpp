#include "../controller/Snare.hpp"
#include "components.hpp"

struct SnareWidget : ModuleWidget {
  SnareWidget(SnareModule *module);
};

SnareWidget::SnareWidget(SnareModule *module) {
	setModule(module);
  box.size = Vec(3 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Snare.svg")));

  addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
  addChild(createWidget<ScrewBlack>(
      Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

  addInput(
      createInput<CDPort>(Vec(10, 30), module, SnareModule::CLOCK1_INPUT));
  addParam(createParam<LightKnobSnap>(
      Vec(10, 72.5), module, SnareModule::DRUM1_PARAM));

  addOutput(createOutput<CDPort>(Vec(10, 120), module,
                                     SnareModule::AUDIO1_OUTPUT));

  addInput(
      createInput<CDPort>(Vec(10, 220), module, SnareModule::CLOCK2_INPUT));
  addParam(createParam<LightKnobSnap>(
      Vec(10, 262.5), module, SnareModule::DRUM2_PARAM));

  addOutput(createOutput<CDPort>(Vec(10, 310), module,
                                     SnareModule::AUDIO2_OUTPUT));
}

Model *modelSnare = createModel<SnareModule, SnareWidget>("SnareDrumN");
