#include "SBD.hpp"
#include <ctime>
#include "../component/tooltips.hpp"

SBDModule::SBDModule( ) {
  config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
  cv = new SynthDevKit::CV(0.5f);
  noise = new SynthDevKit::PinkNoise(time(NULL));
  configParam(SBDModule::PITCH_PARAM, -5.0, 5.0, 0.0, "Pitch", "");
  configParam(SBDModule::PITCH_DECAY_PARAM, -0.2, 1.2, 0.5, "Pitch Decay", " Seconds");
  configParam(SBDModule::AMP_DECAY_PARAM, -5.0, 5.0, 0.0, "Amp Decay", " Seconds");
  configParam<Percent>(SBDModule::SUBOCT_MIX_PARAM, 0.0, 1.0, 0.5, "Mix");
  configParam(SBDModule::DRIVE_PARAM, 0.0, 10.0, 5.0, "Drive");
  configParam(SBDModule::CLICK_PARAM, 0.0, 2.5, 1.25, "Click Level", " Volts");
  configParam<SubOct>(SBDModule::SUBOCT_PARAM, 0.0, 2.0, 0.0, "Sub Octave");
  configParam<WaveShape>(SBDModule::WAVE_PARAM, 0.0, 1.0, 1.0, "Wave");
}

SBDModule::~SBDModule() {
  delete cv;
  delete noise;
}

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
