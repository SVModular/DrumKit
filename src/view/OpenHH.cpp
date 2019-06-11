#include "../controller/OpenHH.hpp"
#include "components.hpp"

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
