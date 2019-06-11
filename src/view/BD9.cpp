#include "../controller/BD9.hpp"

#include "components.hpp"


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
