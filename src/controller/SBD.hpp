#include <cstdint>

#include "../DrumKit.hpp"
#include "../../deps/SynthDevKit/src/CV.hpp"
#include "../../deps/SynthDevKit/src/PinkNoise.hpp"
#include "../../deps/LFO.hpp"
#include "../../deps/ADSR.h"

#define BASE_FREQ 60.0f

struct SBDModule : Module {
  enum ParamIds { PITCH_PARAM, PITCH_DECAY_PARAM, AMP_DECAY_PARAM, SUBOCT_MIX_PARAM, DRIVE_PARAM, CLICK_PARAM, SUBOCT_PARAM, WAVE_PARAM, NUM_PARAMS };
  enum InputIds { GATE_INPUT, PITCH_CV_INPUT, PITCH_DECAY_CV_INPUT, AMP_DECAY_CV_INPUT, NUM_INPUTS };
  enum OutputIds { AUDIO_OUTPUT, NUM_OUTPUTS };
  enum LightIds { NUM_LIGHTS };

  SBDModule( );
  ~SBDModule() override;
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
