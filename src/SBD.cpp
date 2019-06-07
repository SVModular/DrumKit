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

  SBDModule( ) {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    cv = new SynthDevKit::CV(1.7f);
    noise = new SynthDevKit::PinkNoise(time(NULL));
    configParam(SBDModule::PITCH_PARAM, -5.0, 5.0, 0.0);
    configParam(SBDModule::PITCH_DECAY_PARAM, -0.2, 1.2, 0.5);
    configParam(SBDModule::AMP_DECAY_PARAM, -5.0, 5.0, 0.0);
    configParam(SBDModule::SUBOCT_MIX_PARAM, 0.0, 1.0, 0.5);
    configParam(SBDModule::DRIVE_PARAM, 0.0, 10.0, 5.0);
    configParam(SBDModule::CLICK_PARAM, 0.0, 2.5, 1.25);
    configParam(SBDModule::SUBOCT_PARAM, 0.0, 2.0, 0.0);
    configParam(SBDModule::WAVE_PARAM, 0.0, 1.0, 1.0);
  }

  void process(const ProcessArgs &) override;

  LowFrequencyOscillator lfo;
  LowFrequencyOscillator sublfo;
  ADSR freqADSR;
  ADSR ampADSR;
  SynthDevKit::PinkNoise *noise;
  SynthDevKit::CV *cv;
  uint32_t gateCount = 0;
  float playNoise = false;
};

void SBDModule::process(const ProcessArgs &args) {
  float noiseVal = 0.0f;
  float freq_decay = params[PITCH_DECAY_PARAM].getValue();
  float freq_cv = inputs[PITCH_DECAY_CV_INPUT].isConnected() ? inputs[PITCH_DECAY_CV_INPUT].getVoltage() : 0;
  float fd = clamp(freq_decay + freq_cv, 0.0f, 2.2f);

  freqADSR.setAttackRate(.01 * args.sampleRate);
  freqADSR.setDecayRate(fd * args.sampleRate);
  freqADSR.setReleaseRate(1 * args.sampleRate);
  freqADSR.setSustainLevel(.6);

  float amp_decay = params[AMP_DECAY_PARAM].getValue();
  float amp_cv = inputs[AMP_DECAY_CV_INPUT].isConnected() ? inputs[AMP_DECAY_CV_INPUT].getVoltage() : 0;
  float ad = clamp(amp_decay + amp_cv, 0.0f, 2.2f);

  ampADSR.setAttackRate(.01 * args.sampleRate);
  ampADSR.setDecayRate(ad * args.sampleRate);
  ampADSR.setReleaseRate(1 * args.sampleRate);
  ampADSR.setSustainLevel(.6);

  float pitch = params[PITCH_PARAM].getValue();
  float pitch_cv = inputs[PITCH_CV_INPUT].getVoltage();
  float freq = BASE_FREQ + (5 * (pitch + pitch_cv));

  float suboct = params[SUBOCT_PARAM].getValue();

  lfo.setFrequency(freq);
  sublfo.setFrequency(freq / (suboct ? suboct * 2.0f : 1));
  float trigger = inputs[GATE_INPUT].getVoltage();

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

  if (gateCount >= args.sampleRate / 44) {
    gateCount = 0;
    freqADSR.gate(0);
    ampADSR.gate(0);
  }

  if (gateCount >= args.sampleRate / 660) {
    playNoise = false;
  }

  lfo.step(args.sampleTime);
  sublfo.step(args.sampleTime);

  lfo.setFrequency(freq * (1 - freqADSR.process()));
  float lfoo = params[WAVE_PARAM].getValue() ? lfo.sin() : lfo.sqr();

  float sub = sublfo.sqr();
  float mix = params[SUBOCT_MIX_PARAM].getValue();

  float mixed = (sub * mix) + ((1 - mix) * lfoo);
  float drive = params[DRIVE_PARAM].getValue();

  if (playNoise) {
    noiseVal = noise->stepValue() * params[CLICK_PARAM].getValue();
  }

  mixed += noiseVal;

  outputs[AUDIO_OUTPUT].setVoltage(clamp((mixed * ampADSR.process()) * drive, -5.0f, 5.0f));
}

struct SBDWidget : ModuleWidget {
  SBDWidget(SBDModule *module);
};

SBDWidget::SBDWidget(SBDModule *module) {
		setModule(module);
  box.size = Vec(9 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/SBD.svg")));

  addChild(createWidget<ScrewBlack>(Vec(15, 0)));
  addChild(createWidget<ScrewBlack>(Vec(box.size.x-30, 0)));
  addChild(createWidget<ScrewBlack>(Vec(15, 365)));
  addChild(createWidget<ScrewBlack>(Vec(box.size.x-30, 365)));

  addParam(createParam<LightKnobFull>(
      Vec(10, 57.5), module, SBDModule::PITCH_PARAM));
  addInput(createInput<CDPort>(Vec(10, 122.5), module,
                                   SBDModule::PITCH_CV_INPUT));

  addParam(createParam<LightKnobFull>(
      Vec(55, 57.5), module, SBDModule::PITCH_DECAY_PARAM));
  addInput(createInput<CDPort>(Vec(55, 122.5), module,
                                   SBDModule::PITCH_DECAY_CV_INPUT));

  addParam(createParam<LightKnobFull>(
      Vec(100, 57.5), module, SBDModule::AMP_DECAY_PARAM));
  addInput(createInput<CDPort>(Vec(100, 122.5), module,
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

  addInput(createInput<CDPort>(Vec(22, 302.5), module,
                                   SBDModule::GATE_INPUT));
  addOutput(createOutput<CDPort>(Vec(88, 302.5), module,
                                   SBDModule::AUDIO_OUTPUT));

}

Model *modelSBD = createModel<SBDModule, SBDWidget>("SyntheticBassDrum");
