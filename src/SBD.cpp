#include <cstdint>
#include <ctime>

#include "DrumKit.hpp"
#include "../deps/SynthDevKit/src/CV.hpp"
#include "../deps/SynthDevKit/src/PinkNoise.hpp"
#include "../deps/LFO.hpp"
#include "../deps/ADSR.h"

#include "components.hpp"

#define BASE_FREQ 60.0f

struct SBDModule : Module {
  enum ParamIds { PITCH_PARAM, PITCH_DECAY_PARAM, AMP_DECAY_PARAM, SUBOCT_MIX_PARAM, DRIVE_PARAM, CLICK_PARAM, SUBOCT_PARAM, WAVE_PARAM, NUM_PARAMS };
  enum InputIds { GATE_INPUT, PITCH_CV_INPUT, PITCH_DECAY_CV_INPUT, AMP_DECAY_CV_INPUT, NUM_INPUTS };
  enum OutputIds { AUDIO_OUTPUT, NUM_OUTPUTS };
  enum LightIds { NUM_LIGHTS };

  SBDModule( ) : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
    cv = new SynthDevKit::CV(1.7f);
    noise = new SynthDevKit::PinkNoise(time(NULL));
  }

  void step( ) override;

  LowFrequencyOscillator lfo;
  LowFrequencyOscillator sublfo;
  ADSR freqADSR;
  ADSR ampADSR;
  SynthDevKit::PinkNoise *noise;
  SynthDevKit::CV *cv;
  uint32_t gateCount = 0;
  float playNoise = false;
};

void SBDModule::step( ) {
  float noiseVal = 0.0f;
  float freq_decay = params[PITCH_DECAY_PARAM].value;
  float freq_cv = inputs[PITCH_DECAY_CV_INPUT].active ? inputs[PITCH_DECAY_CV_INPUT].value : 0;
  float fd = clamp(freq_decay + freq_cv, 0.0f, 2.2f);

  freqADSR.setAttackRate(.01 * engineGetSampleRate());
  freqADSR.setDecayRate(fd * engineGetSampleRate());
  freqADSR.setReleaseRate(1 * engineGetSampleRate());
  freqADSR.setSustainLevel(.6);

  float amp_decay = params[AMP_DECAY_PARAM].value;
  float amp_cv = inputs[AMP_DECAY_CV_INPUT].active ? inputs[AMP_DECAY_CV_INPUT].value : 0;
  float ad = clamp(amp_decay + amp_cv, 0.0f, 2.2f);

  ampADSR.setAttackRate(.01 * engineGetSampleRate());
  ampADSR.setDecayRate(ad * engineGetSampleRate());
  ampADSR.setReleaseRate(1 * engineGetSampleRate());
  ampADSR.setSustainLevel(.6);

  float pitch = params[PITCH_PARAM].value;
  float pitch_cv = inputs[PITCH_CV_INPUT].value;
  float freq = BASE_FREQ + (5 * (pitch + pitch_cv));

  float suboct = params[SUBOCT_PARAM].value;

  lfo.setFrequency(freq);
  sublfo.setFrequency(freq / (suboct ? suboct * 2.0f : 1));
  float trigger = inputs[GATE_INPUT].value;

  cv->update(trigger);
  if (cv->newTrigger()) {
    gateCount = 0;
    freqADSR.reset();
    ampADSR.reset();
    freqADSR.gate(1);
    ampADSR.gate(1);
    playNoise = true;
  }

  gateCount++;

  if (gateCount >= engineGetSampleRate() / 44) {
    gateCount = 0;
    freqADSR.gate(0);
    ampADSR.gate(0);
  }

  if (gateCount >= engineGetSampleRate() / 660) {
    playNoise = false;
  }

  lfo.step(engineGetSampleTime());
  sublfo.step(engineGetSampleTime());

  lfo.setFrequency(freq * (1 - freqADSR.process()));
  float lfoo = params[WAVE_PARAM].value ? lfo.sin() : lfo.sqr();

  float sub = sublfo.sqr();
  float mix = params[SUBOCT_MIX_PARAM].value;

  float mixed = (sub * mix) + ((1 - mix) * lfoo);
  float drive = params[DRIVE_PARAM].value;

  if (playNoise) {
    noiseVal = noise->stepValue() * params[CLICK_PARAM].value;
  }

  mixed += noiseVal;

  outputs[AUDIO_OUTPUT].value = clamp((mixed * ampADSR.process()) * drive, -5.0f, 5.0f);
}

struct SBDWidget : ModuleWidget {
  SBDWidget(SBDModule *module);
};

SBDWidget::SBDWidget(SBDModule *module) : ModuleWidget(module) {
  box.size = Vec(9 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  {
    SVGPanel *panel = new SVGPanel( );
    panel->box.size = box.size;
    panel->setBackground(SVG::load(assetPlugin(plugin, "res/SBD.svg")));
    addChild(panel);
  }

  addChild(Widget::create<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
  addChild(Widget::create<ScrewBlack>(
      Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));


  addParam(ParamWidget::create<LightKnobFull>(
      Vec(10, 57.5), module, SBDModule::PITCH_PARAM, -5.0, 5.0, 0.0));
  addInput(Port::create<CDPort>(Vec(10, 122.5), Port::INPUT, module,
                                   SBDModule::PITCH_CV_INPUT));

  addParam(ParamWidget::create<LightKnobFull>(
      Vec(55, 57.5), module, SBDModule::PITCH_DECAY_PARAM, -0.2, 1.2, 0.5));
  addInput(Port::create<CDPort>(Vec(55, 122.5), Port::INPUT, module,
                                   SBDModule::PITCH_DECAY_CV_INPUT));

  addParam(ParamWidget::create<LightKnobFull>(
      Vec(100, 57.5), module, SBDModule::AMP_DECAY_PARAM, -5.0, 5.0, 0.0));
  addInput(Port::create<CDPort>(Vec(100, 122.5), Port::INPUT, module,
                                   SBDModule::AMP_DECAY_CV_INPUT));

  addParam(ParamWidget::create<LightKnobFull>(
      Vec(10, 177.5), module, SBDModule::SUBOCT_MIX_PARAM, 0.0, 1.0, 0.5));

  addParam(ParamWidget::create<LightKnobFull>(
      Vec(55, 177.5), module, SBDModule::DRIVE_PARAM, 0.0, 10.0, 5.0));

  addParam(ParamWidget::create<LightKnobFull>(
      Vec(100, 177.5), module, SBDModule::CLICK_PARAM, 0.0, 2.5, 1.25));

  addParam(ParamWidget::create<CKSSThree>(
      Vec(22, 231.5), module, SBDModule::SUBOCT_PARAM, 0.0, 2.0, 0.0));

  addParam(ParamWidget::create<CKSS>(
      Vec(89.5, 236.5), module, SBDModule::WAVE_PARAM, 0.0, 1.0, 1.0));

  addInput(Port::create<CDPort>(Vec(22, 302.5), Port::INPUT, module,
                                   SBDModule::GATE_INPUT));
  addOutput(Port::create<CDPort>(Vec(88, 302.5), Port::OUTPUT, module,
                                   SBDModule::AUDIO_OUTPUT));

}

Model *modelSBD = Model::create<SBDModule, SBDWidget>("DrumKit", "Synthetic Bass Drum", "Synthetic Bass Drum", DRUM_TAG);
