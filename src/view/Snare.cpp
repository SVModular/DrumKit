#include "../controller/Snare.hpp"
#include "../component/port.hpp"
#include "../component/knob.hpp"

struct SnareWidget : ModuleWidget {
  SnareWidget(SnareModule *module);
};

SnareWidget::SnareWidget(SnareModule *module) {
	setModule(module);
  box.size = Vec(6 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Snare.svg")));

  addInput(
      createInput<DKPort>(Vec(10, 30), module, SnareModule::DRUM_CV));

  addInput(
      createInput<DKPort>(Vec(55, 30), module, SnareModule::TUNE_CV));

  addParam(createParam<LightKnobSnap>(
      Vec(10, 72.5), module, SnareModule::DRUM_PARAM));

  addParam(createParam<LightKnob>(
      Vec(55, 72.5), module, SnareModule::TUNE_PARAM));

  addInput(
      createInput<DKPort>(Vec(10, 120), module, SnareModule::GATE_INPUT));

  addOutput(
      createOutput<DKPort>(Vec(55, 120), module, SnareModule::AUDIO_OUTPUT));


  addInput(
      createInput<DKPort>(Vec(10, 220), module, SnareModule::DRUM_CV + 1));

  addInput(
      createInput<DKPort>(Vec(55, 220), module, SnareModule::TUNE_CV + 1));

  addParam(createParam<LightKnobSnap>(
      Vec(10, 262.5), module, SnareModule::DRUM_PARAM + 1));

  addParam(createParam<LightKnob>(
      Vec(55, 262.5), module, SnareModule::TUNE_PARAM + 1));

  addInput(
      createInput<DKPort>(Vec(10, 310), module, SnareModule::GATE_INPUT + 1));

  addOutput(
      createOutput<DKPort>(Vec(55, 310), module, SnareModule::AUDIO_OUTPUT + 1));
}

Model *modelSnare = createModel<SnareModule, SnareWidget>("SnareDrumN");
