#include <cstdint>

#include "../DrumKit.hpp"

struct CR78Container {
  float *sample;
  unsigned int length;
  int current;
};

struct CR78Module : Module {
  enum ParamIds { NUM_PARAMS };
  enum InputIds { NOTE1_INPUT, NOTE2_INPUT, NUM_INPUTS };
  enum OutputIds { AUDIO1_OUTPUT, AUDIO2_OUTPUT, NUM_OUTPUTS };
  enum LightIds { NUM_LIGHTS };

  CR78Module( );

  void process(const ProcessArgs &args) override;

  uint32_t currentStep1;
  int last1;
  uint32_t currentStep2;
  int last2;
};
