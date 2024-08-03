#include "Baronial.hpp"
#include "../component/tooltips.hpp"

float BaronialModule::paramValue (uint16_t param, uint16_t input, float low, float high) {
  float current = params[param].value;

  if (inputs[input].active) {
    // high - low, divided by one tenth input voltage, plus the current value
    current += ((inputs[input].value / 10) * (high - low));
  }

  return clamp(current, low, high);
}

BaronialModule::BaronialModule() {
  config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
  gate = new SynthDevKit::CV(0.5);

  configParam(ATTACK_TIME_PARAM, 0.001, 4.999, 1.0, "Time", " Seconds");
  configParam<Slope>(ATTACK_CURVE_PARAM, 0.0, 1.0, 1.0);
  configParam(DECAY_TIME_PARAM, 0.001, 4.999, 1.0, "Time", " Seconds");
  configParam<Slope>(DECAY_CURVE_PARAM, 0.0, 1.0, 1.0);
  configParam(SUSTAIN_TIME_PARAM, 0.001, 4.999, 1.0, "Time", " Seconds");
  configParam(SUSTAIN_LEVEL_PARAM, 0.001, 4.999, 1.0, "Level", " Volts");
  configParam(RELEASE_TIME_PARAM, 0.001, 4.999, 1.0, "Time", " Seconds");
  configParam<Slope>(RELEASE_CURVE_PARAM, 0.0, 1.0, 1.0);
}

BaronialModule::~BaronialModule() {
  delete gate;
}

void BaronialModule::process(const ProcessArgs &args) {
  if (inputs[GATE].isConnected()) {
    gate->update(inputs[GATE].getVoltage());
  }

  if (gate->newTrigger()) {
    envelope.trigger();
  }

  envelope.setAttackTime(paramValue(ATTACK_TIME_PARAM, ATTACK_TIME_CV, 0.001, 4.999) * args.sampleRate);
  envelope.setDecayTime(paramValue(DECAY_TIME_PARAM, DECAY_TIME_CV, 0.001, 4.999) * args.sampleRate);
  envelope.setSustainTime(paramValue(SUSTAIN_TIME_PARAM, SUSTAIN_TIME_CV, 0.001, 4.999) * args.sampleRate);
  envelope.setReleaseTime(paramValue(RELEASE_TIME_PARAM, RELEASE_TIME_CV, 0.001, 4.999) * args.sampleRate);
  envelope.setSustainLevel(paramValue(SUSTAIN_LEVEL_PARAM, SUSTAIN_LEVEL_CV, 0.01, 0.99));

  envelope.setAttackCurve((CurveType) params[ATTACK_CURVE_PARAM].value);
  envelope.setDecayCurve((CurveType) params[DECAY_CURVE_PARAM].value);
  envelope.setReleaseCurve((CurveType) params[RELEASE_CURVE_PARAM].value);
  envelope.setPeakLevel(1);

  outputs[OUT].setVoltage(envelope.step() * 5);
}
