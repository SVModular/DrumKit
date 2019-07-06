#include "../controller/OpenHH.hpp"
#include "../component/port.hpp"
#include "../component/knob.hpp"

struct OpenHHWidget : ModuleWidget {
  OpenHHWidget(OpenHHModule *module);
};

OpenHHWidget::OpenHHWidget(OpenHHModule *module) {
	setModule(module);
  box.size = Vec(9 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/OpenHH.svg")));

  addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
  addChild(createWidget<ScrewBlack>(
      Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

  addInput(
      createInput<DKPort>(Vec(10, 30), module, OpenHHModule::DRUM_CV));

  addInput(
      createInput<DKPort>(Vec(55, 30), module, OpenHHModule::TUNE_CV));

  addParam(createParam<LightKnobSnap>(
      Vec(10, 72.5), module, OpenHHModule::DRUM_PARAM));

  addParam(createParam<LightKnob>(
      Vec(55, 72.5), module, OpenHHModule::TUNE_PARAM));

  addInput(
      createInput<DKPort>(Vec(10, 120), module, OpenHHModule::GATE_INPUT));

  addOutput(
      createOutput<DKPort>(Vec(55, 120), module, OpenHHModule::AUDIO_OUTPUT));


  addInput(
      createInput<DKPort>(Vec(10, 220), module, OpenHHModule::DRUM_CV + 1));

  addInput(
      createInput<DKPort>(Vec(55, 220), module, OpenHHModule::TUNE_CV + 1));

  addParam(createParam<LightKnobSnap>(
      Vec(10, 262.5), module, OpenHHModule::DRUM_PARAM + 1));

  addParam(createParam<LightKnob>(
      Vec(55, 262.5), module, OpenHHModule::TUNE_PARAM + 1));

  addInput(
      createInput<DKPort>(Vec(10, 310), module, OpenHHModule::GATE_INPUT + 1));

  addOutput(
      createOutput<DKPort>(Vec(55, 310), module, OpenHHModule::AUDIO_OUTPUT + 1));

  addInput(
      createInput<DKPort>(Vec(100, 30), module, OpenHHModule::CHOKE_CV));
  addInput(
      createInput<DKPort>(Vec(100, 220), module, OpenHHModule::CHOKE_CV + 1));


  addParam(createParam<LightKnob>(Vec(100, 72.5), module, OpenHHModule::CHOKE_PARAM));
  addParam(createParam<LightKnob>(Vec(100, 262.5), module, OpenHHModule::CHOKE_PARAM + 1));

  addInput(
      createInput<DKPort>(Vec(100, 120), module, OpenHHModule::CHOKE_GATE));
  addInput(
      createInput<DKPort>(Vec(100, 310), module, OpenHHModule::CHOKE_GATE + 1));
}

Model *modelOpenHH = createModel<OpenHHModule, OpenHHWidget>("OpenHiHat");
