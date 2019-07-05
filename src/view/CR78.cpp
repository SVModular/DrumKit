#include "../controller/CR78.hpp"
#include "../component/port.hpp"
#include "../component/knob.hpp"

struct CR78Widget : ModuleWidget {
  CR78Widget(CR78Module *module);
};

CR78Widget::CR78Widget(CR78Module *module) {
	setModule(module);
  box.size = Vec(3 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/CR78.svg")));

  addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
  addChild(createWidget<ScrewBlack>(
      Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

  addInput(createInput<DKPort>(Vec(10, 30), module,
                                   CR78Module::NOTE1_INPUT));
  addOutput(createOutput<DKPort>(Vec(10, 120), module,
                                     CR78Module::AUDIO1_OUTPUT));
  addInput(createInput<DKPort>(Vec(10, 220), module,
                                   CR78Module::NOTE2_INPUT));
  addOutput(createOutput<DKPort>(Vec(10, 310), module,
                                     CR78Module::AUDIO2_OUTPUT));

}

Model *modelCR78 = createModel<CR78Module, CR78Widget>("CR78");
