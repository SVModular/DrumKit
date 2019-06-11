#include "../controller/ClosedHH.hpp"

#include "components.hpp"

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
