#include "../controller/SBD.hpp"
#include "../component/port.hpp"
#include "../component/knob.hpp"

struct SBDWidget : ModuleWidget {
  SBDWidget(SBDModule *module);
};

SBDWidget::SBDWidget(SBDModule *module) {
	setModule(module);
  box.size = Vec(9 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/SBD.svg")));


  addParam(createParam<LightKnobFull>(
      Vec(10, 57.5), module, SBDModule::PITCH_PARAM));
  addInput(createInput<DKPort>(Vec(10, 122.5), module,
                                   SBDModule::PITCH_CV_INPUT));

  addParam(createParam<LightKnobFull>(
      Vec(55, 57.5), module, SBDModule::PITCH_DECAY_PARAM));
  addInput(createInput<DKPort>(Vec(55, 122.5), module,
                                   SBDModule::PITCH_DECAY_CV_INPUT));

  addParam(createParam<LightKnobFull>(
      Vec(100, 57.5), module, SBDModule::AMP_DECAY_PARAM));
  addInput(createInput<DKPort>(Vec(100, 122.5), module,
                                   SBDModule::AMP_DECAY_CV_INPUT));

  addParam(createParam<LightKnobFull>(
      Vec(10, 177.5), module, SBDModule::SUBOCT_MIX_PARAM));

  addParam(createParam<LightKnobFull>(
      Vec(55, 177.5), module, SBDModule::DRIVE_PARAM));

  addParam(createParam<LightKnobFull>(
      Vec(100, 177.5), module, SBDModule::CLICK_PARAM));

  addParam(createParam<CKSSThree>(
      Vec(22, 231.5), module, SBDModule::SUBOCT_PARAM));

  addParam(createParam<CKSS>(
      Vec(89.5, 236.5), module, SBDModule::WAVE_PARAM));

  addInput(createInput<DKPort>(Vec(22, 302.5), module,
                                   SBDModule::GATE_INPUT));
  addOutput(createOutput<DKPort>(Vec(88, 302.5), module,
                                   SBDModule::AUDIO_OUTPUT));

}

Model *modelSBD = createModel<SBDModule, SBDWidget>("SyntheticBassDrum");
