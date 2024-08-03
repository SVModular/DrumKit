#include "Marionette.hpp"
#include "../model/Envelope.hpp"
#include "../component/tooltips.hpp"

MarionetteModule::MarionetteModule( ) {
  config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
  configParam(PITCH_DECAY_PARAM, 0.1, 2.2, 0.4, "Decay", " Seconds");
  configParam(PITCH_SUSTAIN_PARAM, 0.1, 0.9, 0.2, "Sustain", " Seconds");
  configParam(PITCH_RELEASE_PARAM, 0.1, 2.9, 0.3, "Release", " Seconds");
  configParam<Direction>(PITCH_DECAY_DIR_PARAM, 0.0, 1.0, 0.0, "Decay Direction");
  configParam(AMP_DECAY_PARAM, 0.1, 2.2, 0.4, "Decay", " Seconds");
  configParam(AMP_SUSTAIN_PARAM, 0.1, 0.9, 0.5, "Sustain", " Seconds");
  configParam(AMP_RELEASE_PARAM, 0.1, 2.9, 0.4, "Release", " Seconds");
  configParam<Direction>(AMP_DECAY_DIR_PARAM, 0.0, 1.0, 0.0, "Decay Direction");
  configParam<PercentTen>(BLEND_PARAM, 0, 10, 5, "Blend");
  configParam(TUNE_PARAM, 0.2, 1.8, 1, "Tune");
  configParam<PercentTen>(SUBOCT_MIX_PARAM, 0, 5, 2.5, "Mix");
  configParam<WaveMix>(SUBOCT_WAVE_PARAM, 0, 10, 5, "Square/Sine");
  configParam<SubOct>(SUBOCT_OCT_PARAM, 0, 2, 1, "Sub Octave");
  configParam(KICK_PARAM, 1, 2, 1, "Sample");

  gate = new SynthDevKit::CV(0.5);
  sampleManager = sampleManager->getInstance();

  ctx.sample = sampleManager->selectSample("kick01");
  ctx.currentPosition = ctx.sample->length + 1;
  freqAdsr.setReleaseCurve(CURVED);
  freqAdsr.setDecayCurve(CURVED);
}

MarionetteModule::~MarionetteModule() {
  delete gate;
}

float MarionetteModule::paramValue (uint16_t param, uint16_t input, float low, float high) {
  float current = params[param].getValue();

  if (inputs[input].isConnected()) {
    // high - low, divided by one tenth input voltage, plus the current value
    current += ((inputs[input].getVoltage() / 10) * (high - low));
  }

  return clamp(current, low, high);
}

float MarionetteModule::pitchEnvelope ( ) {
  // if there is a cable plugged into the pitch envelope input, use it
  // division by 5 helps bring us down to a nominal -1 to 1, but realisticly
  // any value should "work", but may not be desirable
  if (inputs[PITCH_ENV_IN].isConnected()) {
    return inputs[PITCH_ENV_IN].getVoltage() / 5;
  }

  float decay = paramValue(PITCH_DECAY_PARAM, PITCH_DECAY_CV_IN, 0, 2.2);
  float sustain = paramValue(PITCH_SUSTAIN_PARAM, PITCH_SUSTAIN_CV_IN, 0.1, 0.9);
  float release = paramValue(PITCH_RELEASE_PARAM, PITCH_RELEASE_CV_IN, 0.1, 2.9);
  freqAdsr.setAttackTime(1);
  freqAdsr.setPeakLevel(1);
  freqAdsr.setDecayTime(decay * APP->engine->getSampleRate());
  freqAdsr.setReleaseTime(release * APP->engine->getSampleRate());
  freqAdsr.setSustainLevel(sustain);
  freqAdsr.setSustainTime(4400);

  // retrieve the ADSR output just in case there's the envelope cable becomes unplugged
  float pe = freqAdsr.step();

  if (params[PITCH_DECAY_PARAM].getValue() == 1) {
    return 1 - pe;
  }

  return pe;
}

float MarionetteModule::ampEnvelope ( ) {
  // if there is a cable plugged into the amp envelope input, use it
  // division by 5 helps bring us down to a nominal -1 to 1, but realisticly
  // any value should "work", but may not be desirable
  if (inputs[AMP_ENV_IN].active) {
    return inputs[AMP_ENV_IN].value / 5;
  }

  float decay = paramValue(AMP_DECAY_PARAM, AMP_DECAY_CV_IN, 0, 2.2);
  float sustain = paramValue(AMP_SUSTAIN_PARAM, AMP_SUSTAIN_CV_IN, 0.1, 0.9);
  float release = paramValue(AMP_RELEASE_PARAM, AMP_RELEASE_CV_IN, 0.1, 2.9);
  ampAdsr.setAttackTime(1);
  ampAdsr.setPeakLevel(1);
  ampAdsr.setDecayTime(decay * APP->engine->getSampleRate());
  ampAdsr.setReleaseTime(release * APP->engine->getSampleRate());
  ampAdsr.setSustainLevel(sustain);
  ampAdsr.setSustainTime(4400);

  // retrieve the ADSR output just in case there's the envelope cable becomes unplugged
  float ae = ampAdsr.step();

  if (params[AMP_DECAY_PARAM].getValue() == 1) {
    return 1 - ae;
  }

  return ae;
}

float MarionetteModule::subOctValue(float sampleTime, float freq, float amp) {
  float suboct = params[SUBOCT_OCT_PARAM].getValue();
  sublfo.setFrequency(freq / (suboct ? suboct * 2.0f : 1));
  sublfo.step(sampleTime);

  float sqr = sublfo.sqr();
  float sin = sublfo.sin();

  float blend = paramValue(SUBOCT_WAVE_PARAM, SUBOCT_WAVE_CV, 0, 10);
  float blended = (sin * (blend / 10)) + (sqr * (1 - (blend / 10)));

  return blended * amp;
}

void MarionetteModule::process(const ProcessArgs &args) {
  if (inputs[GATE].isConnected()) {
    gate->update(inputs[GATE].getVoltage());
  }

  if (gate->newTrigger()) {
    // turn on the frequency gate
    freqAdsr.trigger();

    // turn on the amplidute gate
    ampAdsr.trigger();

    // reset the context
    ctx.reset();
  }

  // check kick context
  uint8_t sample = (uint8_t) params[KICK_PARAM].getValue();
  ctx.sample = sampleManager->selectSample(sample == 1 ? "kick01" : "kick02");

  float stepValue = pitchEnvelope();

  float tune = paramValue(TUNE_PARAM, TUNE_CV_IN, 0.2, 1.8);

  float sampleValue = sampleManager->step(&ctx, stepValue * tune, args.sampleRate);
  float volume = ampEnvelope();

  float subValue = subOctValue(args.sampleTime, ctx.sample->pitch * stepValue * tune * 2, volume);
  float subMix = paramValue(SUBOCT_MIX_PARAM, SUBOCT_MIX_CV, 0, 10);

  lfo.setFrequency(ctx.sample->pitch * stepValue * 2 * tune);
  lfo.step(args.sampleTime);

  float lfoValue = lfo.sin();

  float mix = paramValue(BLEND_PARAM, BLEND_CV_IN, 0, 10);

  float blended = (sampleValue * (mix / 10)) + ((lfoValue) * (1 - (mix / 10)));

  float subBlended = (subValue * (subMix / 10)) + (blended * (1 - (subMix / 10)));

  outputs[OUT].setVoltage(subBlended * 5 * volume);

}
