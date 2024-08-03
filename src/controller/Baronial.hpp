#include <cstdint>

#include "../DrumKit.hpp"

#include "../../deps/SynthDevKit/src/CV.hpp"
#include "../model/Envelope.hpp"


struct BaronialModule : Module {
  enum ParamIds { ATTACK_TIME_PARAM, DECAY_TIME_PARAM, SUSTAIN_TIME_PARAM, SUSTAIN_LEVEL_PARAM, RELEASE_TIME_PARAM, ATTACK_CURVE_PARAM, DECAY_CURVE_PARAM, RELEASE_CURVE_PARAM, DIRECTION_PARAM, NUM_PARAMS };
  enum InputIds {  GATE, ATTACK_TIME_CV, DECAY_TIME_CV, SUSTAIN_TIME_CV, SUSTAIN_LEVEL_CV, RELEASE_TIME_CV, NUM_INPUTS };
  enum OutputIds { OUT, NUM_OUTPUTS };
  enum LightIds { NUM_LIGHTS };

  BaronialModule();
  ~BaronialModule() override;
  float paramValue (uint16_t, uint16_t, float, float);

  void process(const ProcessArgs &args) override;

  SynthDevKit::CV *gate;
  Envelope envelope;
};
