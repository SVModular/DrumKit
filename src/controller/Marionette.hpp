#include <cstdint>

#include "../DrumKit.hpp"
#include "../../deps/SynthDevKit/src/CV.hpp"
#include "../../deps/LFO.hpp"
#include "../model/Envelope.hpp"
#include "../model/SampleManager.hpp"

using namespace DrumKit;

struct MarionetteModule : Module {
  enum ParamIds {
    PITCH_DECAY_PARAM,
    PITCH_SUSTAIN_PARAM,
    PITCH_RELEASE_PARAM,
    PITCH_DECAY_DIR_PARAM,
    AMP_DECAY_PARAM,
    AMP_SUSTAIN_PARAM,
    AMP_RELEASE_PARAM,
    AMP_DECAY_DIR_PARAM,
    BLEND_PARAM,
    TUNE_PARAM,
    SUBOCT_MIX_PARAM,
    SUBOCT_WAVE_PARAM,
    SUBOCT_OCT_PARAM,
    KICK_PARAM,
    NUM_PARAMS };
  enum InputIds {
    GATE,
    PITCH_ENV_IN,
    PITCH_DECAY_CV_IN,
    PITCH_SUSTAIN_CV_IN,
    PITCH_RELEASE_CV_IN,
    AMP_ENV_IN,
    AMP_ATTACK_CV_IN,
    AMP_DECAY_CV_IN,
    AMP_SUSTAIN_CV_IN,
    AMP_RELEASE_CV_IN,
    BLEND_CV_IN,
    TUNE_CV_IN,
    SUBOCT_MIX_CV,
    SUBOCT_WAVE_CV,
    NUM_INPUTS };
  enum OutputIds { OUT, ENVELOPE_OUT, NUM_OUTPUTS };
  enum LightIds { NUM_LIGHTS };

  MarionetteModule();
  ~MarionetteModule() override;
  void process(const ProcessArgs &) override;

  float pitchEnvelope ( );
  float ampEnvelope ( );
  float paramValue (uint16_t, uint16_t, float, float);
  float subOctValue(float, float, float);

  SynthDevKit::CV *gate;
  Envelope freqAdsr;
  Envelope ampAdsr;

  SampleManagerContext ctx;
  SampleManager *sampleManager = {0};

  LowFrequencyOscillator lfo;
  LowFrequencyOscillator sublfo;
};
