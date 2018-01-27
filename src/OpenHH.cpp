#include <stdint.h>

#include "../deps/SynthDevKit/src/CV.hpp"
#include "DrumKit.hpp"
#include "openhh.h"

struct OpenHHContainer {
  float *sample;
  unsigned int length;
};

struct OpenHHContainer ohhsamples[ 16 ] = {
  { (float *)openhh1, openhh1_len },
  { (float *)openhh2, openhh2_len },
  { (float *)openhh3, openhh3_len },
  { (float *)openhh4, openhh4_len },
  { (float *)openhh5, openhh5_len },
  { (float *)openhh6, openhh6_len },
  { (float *)openhh7, openhh7_len },
  { (float *)openhh8, openhh8_len },
  { (float *)openhh9, openhh9_len },
  { (float *)openhh10, openhh10_len },
  { (float *)openhh11, openhh11_len },
  { (float *)openhh12, openhh12_len },
  { (float *)openhh13, openhh13_len },
  { (float *)openhh14, openhh14_len },
};

struct OpenHHContainer *ohhgetState(float current) {
  if (current < 1) {
    return &ohhsamples[ 0 ];
  }
  return &ohhsamples[ (int)current - 1 ];
}

struct OpenHHModule : Module {
  enum ParamIds { DRUM1_PARAM, DRUM2_PARAM, NUM_PARAMS };
  enum InputIds { CLOCK1_INPUT, CLOCK2_INPUT, NUM_INPUTS };
  enum OutputIds { AUDIO1_OUTPUT, AUDIO2_OUTPUT, NUM_OUTPUTS };
  enum LightIds { NUM_LIGHTS };

  OpenHHModule( ) : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
    cv1          = new SynthDevKit::CV(1.7f);
    cv2          = new SynthDevKit::CV(1.7f);
    currentStep1 = 0;
    currentStep2 = 0;
    ready1       = false;
    ready2       = false;
  }

  void step( ) override;

  SynthDevKit::CV *cv1;
  uint32_t currentStep1;
  bool ready1;
  SynthDevKit::CV *cv2;
  uint32_t currentStep2;
  bool ready2;
};

void OpenHHModule::step( ) {
  float in1 = inputs[ CLOCK1_INPUT ].value;
  cv1->update(in1);

  if (cv1->newTrigger( )) {
    if (!ready1) {
      ready1 = true;
    }

    currentStep1 = 0;
  }

  float current1              = params[ DRUM1_PARAM ].value;
  struct OpenHHContainer *c = ohhgetState(current1);

  if (currentStep1 >= c->length) {
    outputs[ AUDIO1_OUTPUT ].value = 0;
  } else {
    outputs[ AUDIO1_OUTPUT ].value = c->sample[ currentStep1 ];
    currentStep1++;
  }

  float in2 = inputs[ CLOCK2_INPUT ].value;
  cv2->update(in2);

  if (cv2->newTrigger( )) {
    if (!ready2) {
      ready2 = true;
    }

    currentStep2 = 0;
  }

  float current2 = params[ DRUM2_PARAM ].value;
  c              = ohhgetState(current2);

  if (currentStep2 >= c->length) {
    outputs[ AUDIO2_OUTPUT ].value = 0;
  } else {
    outputs[ AUDIO2_OUTPUT ].value = c->sample[ currentStep2 ];
    currentStep2++;
  }
}

OpenHHWidget::OpenHHWidget( ) {
  OpenHHModule *module = new OpenHHModule( );
  setModule(module);
  box.size = Vec(3 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  {
    SVGPanel *panel = new SVGPanel( );
    panel->box.size = box.size;
    panel->setBackground(SVG::load(assetPlugin(plugin, "res/OpenHH.svg")));
    addChild(panel);
  }

  addChild(createScrew<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
  addChild(createScrew<ScrewSilver>(
      Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

  addInput(createInput<PJ301MPort>(Vec(10, 45), module,
                                   OpenHHModule::CLOCK1_INPUT));
  addParam(createParam<RoundSmallBlackSnapKnob>(
      Vec(8, 92), module, OpenHHModule::DRUM1_PARAM, 1.0, 14.0, 7.0));

  addOutput(createOutput<PJ301MPort>(Vec(10, 149), module,
                                     OpenHHModule::AUDIO1_OUTPUT));

  addInput(createInput<PJ301MPort>(Vec(10, 205), module,
                                   OpenHHModule::CLOCK2_INPUT));
  addParam(createParam<RoundSmallBlackSnapKnob>(
      Vec(8, 252), module, OpenHHModule::DRUM2_PARAM, 1.0, 14.0, 7.0));

  addOutput(createOutput<PJ301MPort>(Vec(10, 308), module,
                                     OpenHHModule::AUDIO2_OUTPUT));
}
