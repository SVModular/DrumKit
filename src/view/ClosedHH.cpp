#include "../controller/ClosedHH.hpp"

#include "../component/port.hpp"
#include "../component/knob.hpp"

struct ClosedHHWidget : ModuleWidget {
  ClosedHHWidget(ClosedHHModule *module);
};

ClosedHHWidget::ClosedHHWidget(ClosedHHModule *module) {
	setModule(module);
  box.size = Vec(6 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/ClosedHH.svg")));


  addInput(
      createInput<DKPort>(Vec(10, 30), module, ClosedHHModule::DRUM_CV));

  addInput(
      createInput<DKPort>(Vec(55, 30), module, ClosedHHModule::TUNE_CV));

  addParam(createParam<LightKnobSnap>(
      Vec(10, 72.5), module, ClosedHHModule::DRUM_PARAM));

  addParam(createParam<LightKnob>(
      Vec(55, 72.5), module, ClosedHHModule::TUNE_PARAM));

  addInput(
      createInput<DKPort>(Vec(10, 120), module, ClosedHHModule::GATE_INPUT));

  addOutput(
      createOutput<DKPort>(Vec(55, 120), module, ClosedHHModule::AUDIO_OUTPUT));


  addInput(
      createInput<DKPort>(Vec(10, 220), module, ClosedHHModule::DRUM_CV + 1));

  addInput(
      createInput<DKPort>(Vec(55, 220), module, ClosedHHModule::TUNE_CV + 1));

  addParam(createParam<LightKnobSnap>(
      Vec(10, 262.5), module, ClosedHHModule::DRUM_PARAM + 1));

  addParam(createParam<LightKnob>(
      Vec(55, 262.5), module, ClosedHHModule::TUNE_PARAM + 1));

  addInput(
      createInput<DKPort>(Vec(10, 310), module, ClosedHHModule::GATE_INPUT + 1));

  addOutput(
      createOutput<DKPort>(Vec(55, 310), module, ClosedHHModule::AUDIO_OUTPUT + 1));
}

Model *modelClosedHH = createModel<ClosedHHModule, ClosedHHWidget>("ClosedHiHat");
