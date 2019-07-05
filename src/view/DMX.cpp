#include "../controller/DMX.hpp"
#include "../component/port.hpp"
#include "../component/knob.hpp"

struct DMXWidget : ModuleWidget {
  DMXWidget(DMXModule *module);
};

DMXWidget::DMXWidget(DMXModule *module) {
	setModule(module);
  box.size = Vec(6 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/DMX.svg")));

  addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
  addChild(createWidget<ScrewBlack>(
      Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

  addInput(
      createInput<DKPort>(Vec(10, 30), module, DMXModule::DRUM_CV));

  addInput(
      createInput<DKPort>(Vec(55, 30), module, DMXModule::TUNE_CV));

  addParam(createParam<LightKnobSnap>(
      Vec(10, 72.5), module, DMXModule::DRUM_PARAM));

  addParam(createParam<LightKnob>(
      Vec(55, 72.5), module, DMXModule::TUNE_PARAM));

  addInput(
      createInput<DKPort>(Vec(10, 120), module, DMXModule::GATE_INPUT));

  addOutput(
      createOutput<DKPort>(Vec(55, 120), module, DMXModule::AUDIO_OUTPUT));


  addInput(
      createInput<DKPort>(Vec(10, 220), module, DMXModule::DRUM_CV + 1));

  addInput(
      createInput<DKPort>(Vec(55, 220), module, DMXModule::TUNE_CV + 1));

  addParam(createParam<LightKnobSnap>(
      Vec(10, 262.5), module, DMXModule::DRUM_PARAM + 1));

  addParam(createParam<LightKnob>(
      Vec(55, 262.5), module, DMXModule::TUNE_PARAM + 1));

  addInput(
      createInput<DKPort>(Vec(10, 310), module, DMXModule::GATE_INPUT + 1));

  addOutput(
      createOutput<DKPort>(Vec(55, 310), module, DMXModule::AUDIO_OUTPUT + 1));

}

Model *modelDMX = createModel<DMXModule, DMXWidget>("DMX");
