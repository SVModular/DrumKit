#include "../controller/BD9.hpp"

#include "../component/port.hpp"
#include "../component/knob.hpp"

struct BD9Widget : ModuleWidget {
  BD9Widget(BD9Module *module);
};

BD9Widget::BD9Widget(BD9Module *module) {
	setModule(module);
  box.size = Vec(6 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/BD9.svg")));

  addInput(
      createInput<DKPort>(Vec(10, 30), module, BD9Module::DRUM_CV));

  addInput(
      createInput<DKPort>(Vec(55, 30), module, BD9Module::TUNE_CV));

  addParam(createParam<LightKnobSnap>(
      Vec(10, 72.5), module, BD9Module::DRUM_PARAM));

  addParam(createParam<LightKnob>(
      Vec(55, 72.5), module, BD9Module::TUNE_PARAM));

  addInput(
      createInput<DKPort>(Vec(10, 120), module, BD9Module::GATE_INPUT));

  addOutput(
      createOutput<DKPort>(Vec(55, 120), module, BD9Module::AUDIO_OUTPUT));


  addInput(
      createInput<DKPort>(Vec(10, 220), module, BD9Module::DRUM_CV + 1));

  addInput(
      createInput<DKPort>(Vec(55, 220), module, BD9Module::TUNE_CV + 1));

  addParam(createParam<LightKnobSnap>(
      Vec(10, 262.5), module, BD9Module::DRUM_PARAM + 1));

  addParam(createParam<LightKnob>(
      Vec(55, 262.5), module, BD9Module::TUNE_PARAM + 1));

  addInput(
      createInput<DKPort>(Vec(10, 310), module, BD9Module::GATE_INPUT + 1));

  addOutput(
      createOutput<DKPort>(Vec(55, 310), module, BD9Module::AUDIO_OUTPUT + 1));
}

Model *modelBD9 = createModel<BD9Module, BD9Widget>("BassDrum9");
