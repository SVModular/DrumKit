#include "../controller/Baronial.hpp"
#include "../component/port.hpp"
#include "../component/knob.hpp"

struct BaronialWidget : ModuleWidget {
  BaronialWidget(BaronialModule *module);
};

BaronialWidget::BaronialWidget(BaronialModule *module) {
	setModule(module);
  box.size = Vec(9 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Baronial.svg")));

  // gate in
  addInput(createInput<DKPort>(Vec(10, 314.5), module,
                                 BaronialModule::GATE));


  // attack
  addInput(createInput<DKPort>(Vec(6, 34.5), module,
                                 BaronialModule::ATTACK_TIME_CV));

  addParam(createParam<LightKnobFull>(
       Vec(32.5, 29.5), module, BaronialModule::ATTACK_TIME_PARAM));

  addParam(createParam<CKSS>(
       Vec(65, 34.5), module, BaronialModule::ATTACK_CURVE_PARAM));


  // decay
  addInput(createInput<DKPort>(Vec(6, 94.5), module,
                                 BaronialModule::DECAY_TIME_CV));

  addParam(createParam<LightKnobFull>(
       Vec(32.5, 89.5), module, BaronialModule::DECAY_TIME_PARAM));

  addParam(createParam<CKSS>(
       Vec(65, 94.5), module, BaronialModule::DECAY_CURVE_PARAM));


  // sustain
  addInput(createInput<DKPort>(Vec(10, 154.5), module,
                                 BaronialModule::SUSTAIN_TIME_CV));

  addParam(createParam<LightKnobFull>(
       Vec(55, 149.5), module, BaronialModule::SUSTAIN_TIME_PARAM));

  addInput(createInput<DKPort>(Vec(10, 204.5), module,
                                 BaronialModule::SUSTAIN_LEVEL_CV));

  addParam(createParam<LightKnobFull>(
       Vec(55, 199.5), module, BaronialModule::SUSTAIN_LEVEL_PARAM));


  // release
  addInput(createInput<DKPort>(Vec(6, 264.5), module,
                                 BaronialModule::RELEASE_TIME_CV));

  addParam(createParam<LightKnobFull>(
       Vec(32.5, 259.5), module, BaronialModule::RELEASE_TIME_PARAM));

  addParam(createParam<CKSS>(
       Vec(65, 264.5), module, BaronialModule::RELEASE_CURVE_PARAM));

  // output
  addOutput(createOutput<DKPort>(Vec(55, 314.5), module,
                                BaronialModule::OUT));
}

Model *modelBaronial = createModel<BaronialModule, BaronialWidget>("Baronial");
