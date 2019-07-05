#include "../controller/CR78mk2.hpp"
#include "../component/port.hpp"
#include "../component/knob.hpp"

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

  addInput(createInput<DKPort>(Vec(10, 62.6), module,
                                   CR78mk2Module::VOCT_INPUT));

  addInput(createInput<DKPort>(Vec(10, 122.5), module,
                                   CR78mk2Module::GATE_INPUT));

  addParam(createParam<LightKnobFull>(
      Vec(10, 177.5), module, CR78mk2Module::TUNE_PARAM));
  addInput(createInput<DKPort>(Vec(10, 242.5), module,
                                   CR78mk2Module::TUNE_CV_INPUT));

  addOutput(createOutput<DKPort>(Vec(10, 302.5), module,
                                     CR78mk2Module::AUDIO_OUTPUT));

}

Model *modelCR78mk2 = createModel<CR78mk2Module, CR78mk2Widget>("CR78mk2");
