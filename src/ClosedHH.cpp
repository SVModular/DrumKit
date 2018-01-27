#include <stdint.h>

#include "../deps/SynthDevKit/src/CV.hpp"
#include "DrumKit.hpp"
#include "closedhh.h"

struct ClosedHHContainer {
  float *sample;
  unsigned int length;
};

struct ClosedHHContainer chhsamples[ 16 ] = {
  { (float *)closedhh1, closedhh1_len },
  { (float *)closedhh2, closedhh2_len },
  { (float *)closedhh3, closedhh3_len },
  { (float *)closedhh4, closedhh4_len },
  { (float *)closedhh5, closedhh5_len },
  { (float *)closedhh6, closedhh6_len },
  { (float *)closedhh7, closedhh7_len },
  { (float *)closedhh8, closedhh8_len },
  { (float *)closedhh9, closedhh9_len },
  { (float *)closedhh10, closedhh10_len },
  { (float *)closedhh11, closedhh11_len },
  { (float *)closedhh12, closedhh12_len },
  { (float *)closedhh13, closedhh13_len },
  { (float *)closedhh14, closedhh14_len },
  { (float *)closedhh15, closedhh15_len }
};

struct ClosedHHContainer *chhgetState(float current) {
  if (current < 1) {
    return &chhsamples[ 0 ];
  }
  return &chhsamples[ (int)current - 1 ];
}

struct ClosedHHModule : Module {
  enum ParamIds { DRUM1_PARAM, DRUM2_PARAM, NUM_PARAMS };
  enum InputIds { CLOCK1_INPUT, CLOCK2_INPUT, NUM_INPUTS };
  enum OutputIds { AUDIO1_OUTPUT, AUDIO2_OUTPUT, NUM_OUTPUTS };
  enum LightIds { NUM_LIGHTS };

  ClosedHHModule( ) : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
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

void ClosedHHModule::step( ) {
  float in1 = inputs[ CLOCK1_INPUT ].value;
  cv1->update(in1);

  if (cv1->newTrigger( )) {
    if (!ready1) {
      ready1 = true;
    }

    currentStep1 = 0;
  }

  float current1              = params[ DRUM1_PARAM ].value;
  struct ClosedHHContainer *c = chhgetState(current1);

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
  c              = chhgetState(current2);

  if (currentStep2 >= c->length) {
    outputs[ AUDIO2_OUTPUT ].value = 0;
  } else {
    outputs[ AUDIO2_OUTPUT ].value = c->sample[ currentStep2 ];
    currentStep2++;
  }
}

ClosedHHWidget::ClosedHHWidget( ) {
  ClosedHHModule *module = new ClosedHHModule( );
  setModule(module);
  box.size = Vec(3 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  {
    SVGPanel *panel = new SVGPanel( );
    panel->box.size = box.size;
    panel->setBackground(SVG::load(assetPlugin(plugin, "res/ClosedHH.svg")));
    addChild(panel);
  }

  addChild(createScrew<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
  addChild(createScrew<ScrewSilver>(
      Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

  addInput(createInput<PJ301MPort>(Vec(10, 45), module,
                                   ClosedHHModule::CLOCK1_INPUT));
  addParam(createParam<RoundSmallBlackSnapKnob>(
      Vec(8, 92), module, ClosedHHModule::DRUM1_PARAM, 1.0, 15.0, 8.0));

  addOutput(createOutput<PJ301MPort>(Vec(10, 149), module,
                                     ClosedHHModule::AUDIO1_OUTPUT));

  addInput(createInput<PJ301MPort>(Vec(10, 205), module,
                                   ClosedHHModule::CLOCK2_INPUT));
  addParam(createParam<RoundSmallBlackSnapKnob>(
      Vec(8, 252), module, ClosedHHModule::DRUM2_PARAM, 1.0, 15.0, 8.0));

  addOutput(createOutput<PJ301MPort>(Vec(10, 308), module,
                                     ClosedHHModule::AUDIO2_OUTPUT));
}
