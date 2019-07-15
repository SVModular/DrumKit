#include "../controller/Marionette.hpp"

#include "../component/knob.hpp"
#include "../component/port.hpp"

struct MarionetteWidget : ModuleWidget {
  MarionetteWidget(MarionetteModule *module);
};

MarionetteWidget::MarionetteWidget(MarionetteModule *module) {
  setModule(module);
  box.size = Vec(15 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Marionette.svg")));


  // pitch envelope
  // decay
  addParam(createParam<LightKnobFull>(
       Vec(55, 29.5), module, MarionetteModule::PITCH_DECAY_PARAM));

  addInput(createInput<DKPort>(Vec(6, 34.5), module,
                                 MarionetteModule::PITCH_DECAY_CV_IN));


  // sustain
  addParam(createParam<LightKnobFull>(
       Vec(55, 79.5), module, MarionetteModule::PITCH_SUSTAIN_PARAM));

  addInput(createInput<DKPort>(Vec(6, 84.5), module,
                                 MarionetteModule::PITCH_SUSTAIN_CV_IN));


  // release
  addParam(createParam<LightKnobFull>(
       Vec(55, 129.5), module, MarionetteModule::PITCH_RELEASE_PARAM));

  addInput(createInput<DKPort>(Vec(6, 134.5), module,
                                 MarionetteModule::PITCH_RELEASE_CV_IN));

  addParam(createParam<CKSS>(Vec(38, 182.5), module, MarionetteModule::PITCH_DECAY_DIR_PARAM));

  // frequency envelope input
  addInput(createInput<DKPort>(Vec(32.5, 224.5), module,
                                 MarionetteModule::PITCH_ENV_IN));


  // amplitude envelope
  // decay
  addParam(createParam<LightKnobFull>(
       Vec(190, 29.5), module, MarionetteModule::AMP_DECAY_PARAM));

  addInput(createInput<DKPort>(Vec(141, 34.5), module,
                                 MarionetteModule::AMP_DECAY_CV_IN));


  // sustain
  addParam(createParam<LightKnobFull>(
       Vec(190, 79.5), module, MarionetteModule::AMP_SUSTAIN_PARAM));

  addInput(createInput<DKPort>(Vec(141, 84.5), module,
                                 MarionetteModule::AMP_SUSTAIN_CV_IN));


  // release
  addParam(createParam<LightKnobFull>(
       Vec(190, 129.5), module, MarionetteModule::AMP_RELEASE_PARAM));

  addInput(createInput<DKPort>(Vec(141, 134.5), module,
                                 MarionetteModule::AMP_RELEASE_CV_IN));

  // amplitude envelope input
  addInput(createInput<DKPort>(Vec(167.5, 224.5), module,
                                 MarionetteModule::AMP_ENV_IN));

  addParam(createParam<CKSS>(Vec(173, 182.5), module, MarionetteModule::AMP_DECAY_DIR_PARAM));


  // gate in
  addInput(createInput<DKPort>(Vec(100, 34.5), module,
                                 MarionetteModule::GATE));

  // tune
  addParam(createParam<LightKnobFull>(
       Vec(109.5, 129.5), module, MarionetteModule::TUNE_PARAM));
  addInput(createInput<DKPort>(Vec(85, 134.5), module, MarionetteModule::TUNE_CV_IN));

  // blend
  addParam(createParam<LightKnobFull>(
       Vec(109.5, 179.5), module, MarionetteModule::BLEND_PARAM));
  addInput(createInput<DKPort>(Vec(85, 184.5), module, MarionetteModule::BLEND_CV_IN));

  // output
  addOutput(createOutput<DKPort>(Vec(100, 84.5), module,
                                MarionetteModule::OUT));

  // kick select
  addParam(createParam<LightKnobSnap>(
       Vec(100.5, 229.5), module, MarionetteModule::KICK_PARAM));

  // sub mix
  addParam(createParam<LightKnobFull>(Vec(55, 289.5), module, MarionetteModule::SUBOCT_MIX_PARAM));
  addInput(createInput<DKPort>(Vec(6, 294.5), module, MarionetteModule::SUBOCT_MIX_CV));

  // sub octave
  addParam(createParam<CKSSThree>(Vec(105.5, 289), module, MarionetteModule::SUBOCT_OCT_PARAM));

  // sub wave
  addParam(createParam<LightKnobFull>(Vec(190, 289.5), module, MarionetteModule::SUBOCT_WAVE_PARAM));
  addInput(createInput<DKPort>(Vec(141, 294.5), module, MarionetteModule::SUBOCT_WAVE_CV));
}

Model *modelMarionette = createModel<MarionetteModule, MarionetteWidget>("MarionetteBass");
