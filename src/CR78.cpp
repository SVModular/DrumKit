#include <stdint.h>

#include "DrumKit.hpp"
#include "cr78.h"

#include "components.hpp"

unsigned int cr781_len = 5672;
unsigned int cr782_len = 3064;
unsigned int cr783_len = 5840;
unsigned int cr784_len = 15105;
unsigned int cr785_len = 2356;
unsigned int cr786_len = 2577;
unsigned int cr787_len = 18005;

struct CR78Container {
  float *sample;
  unsigned int length;
  int current;
};

struct CR78Container cr78samples[ 7 ] = {
  { (float *)cr781, cr781_len, 0 },
  { (float *)cr782, cr782_len, 1 },
  { (float *)cr783, cr783_len, 2 },
  { (float *)cr784, cr784_len, 3 },
  { (float *)cr785, cr785_len, 4 },
  { (float *)cr786, cr786_len, 5 },
  { (float *)cr787, cr787_len, 6 }
};

float cr78notes[7] = { 0.08, 0.17, 0.25, 0.33, 0.42, 0.5, 0.58 };


struct CR78Container *getCR78Note(float current) {
  for (int i = 0; i < 7; i++) {
    if ((cr78notes[i] - 0.02) <= current && (cr78notes[i] + 0.02) >= current) {
      return &cr78samples[i];
    }
  }

  return NULL;
}

struct CR78Module : Module {
  enum ParamIds { NUM_PARAMS };
  enum InputIds { NOTE1_INPUT, NOTE2_INPUT, NUM_INPUTS };
  enum OutputIds { AUDIO1_OUTPUT, AUDIO2_OUTPUT, NUM_OUTPUTS };
  enum LightIds { NUM_LIGHTS };

  CR78Module( ) {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    currentStep1 = 0;
    last1 = -1;
    currentStep2 = 0;
    last2 = -1;
  }

  void step( ) override;

  uint32_t currentStep1;
  int last1;
  uint32_t currentStep2;
  int last2;
};

void CR78Module::step( ) {
  float in1 = inputs[ NOTE1_INPUT ].getVoltage();
  struct CR78Container *note1;

  // check the first note
  note1 = getCR78Note(in1);

  if (note1 == NULL) {
    currentStep1 = 0;
    outputs[ AUDIO1_OUTPUT ].setVoltage(0);
    last1 = -1;
  } else {
    if (last1 != note1->current) {
      last1 = note1->current;
      currentStep1 = 0;
    }

    if (currentStep1 >= note1->length) {
      outputs[ AUDIO1_OUTPUT ].setVoltage(0);
    } else {
      outputs[ AUDIO1_OUTPUT ].setVoltage(note1->sample[currentStep1]);
      currentStep1++;
    }
  }

  float in2 = inputs[ NOTE2_INPUT ].getVoltage();
  struct CR78Container *note2;

  // check the first note
  note2 = getCR78Note(in2);

  if (note2 == NULL) {
    currentStep2 = 0;
    outputs[ AUDIO2_OUTPUT ].setVoltage(0);
    last2 = -1;
  } else {
    if (last2 != note2->current) {
      last2 = note2->current;
      currentStep2 = 0;
    }

    if (currentStep2 >= note2->length) {
      outputs[ AUDIO2_OUTPUT ].setVoltage(0);
    } else {
      outputs[ AUDIO2_OUTPUT ].setVoltage(note2->sample[currentStep2]);
      currentStep2++;
    }
  }

}

struct CR78Widget : ModuleWidget {
  CR78Widget(CR78Module *module);
};

CR78Widget::CR78Widget(CR78Module *module) {
		setModule(module);
  box.size = Vec(3 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/CR78.svg")));

  addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
  addChild(createWidget<ScrewBlack>(
      Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

  addInput(createInput<CDPort>(Vec(10, 30), module,
                                   CR78Module::NOTE1_INPUT));
  addOutput(createOutput<CDPort>(Vec(10, 120), module,
                                     CR78Module::AUDIO1_OUTPUT));
  addInput(createInput<CDPort>(Vec(10, 220), module,
                                   CR78Module::NOTE2_INPUT));
  addOutput(createOutput<CDPort>(Vec(10, 310), module,
                                     CR78Module::AUDIO2_OUTPUT));

}

Model *modelCR78 = createModel<CR78Module, CR78Widget>("CR78");
