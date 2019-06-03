#include <stdint.h>

#include "DrumKit.hpp"
#include "dmx.h"

#include "components.hpp"

unsigned int dmx1_len = 7689;
unsigned int dmx2_len = 61190;
unsigned int dmx3_len = 5947;
unsigned int dmx4_len = 5831;
unsigned int dmx5_len = 48148;
unsigned int dmx6_len = 4110;
unsigned int dmx7_len = 7683;
unsigned int dmx8_len = 8187;
unsigned int dmx9_len = 3664;
unsigned int dmx10_len = 31345;
unsigned int dmx11_len = 4941;
unsigned int dmx12_len = 6215;

struct DMXContainer {
  float *sample;
  unsigned int length;
  int current;
};

struct DMXContainer dmxsamples[ 12 ] = {
  { (float *)dmx1, dmx1_len, 0 },
  { (float *)dmx2, dmx2_len, 1 },
  { (float *)dmx3, dmx3_len, 2 },
  { (float *)dmx4, dmx4_len, 3 },
  { (float *)dmx5, dmx5_len, 4 },
  { (float *)dmx6, dmx6_len, 5 },
  { (float *)dmx7, dmx7_len, 6 },
  { (float *)dmx8, dmx8_len, 7 },
  { (float *)dmx9, dmx9_len, 8 },
  { (float *)dmx10, dmx10_len, 9 },
  { (float *)dmx11, dmx11_len, 10 },
  { (float *)dmx12, dmx12_len, 11 },
};

float dmxnotes[12] = { 0.08, 0.17, 0.25, 0.33, 0.42, 0.5, 0.58, 0.67, 0.75, 0.83, 0.92, 1.0 };


struct DMXContainer *getNote(float current) {
  for (int i = 0; i < 12; i++) {
    if ((dmxnotes[i] - 0.02) <= current && (dmxnotes[i] + 0.02) >= current) {
      return &dmxsamples[i];
    }
  }

  return NULL;
}

struct DMXModule : Module {
  enum ParamIds { NUM_PARAMS };
  enum InputIds { NOTE1_INPUT, NOTE2_INPUT, NUM_INPUTS };
  enum OutputIds { AUDIO1_OUTPUT, AUDIO2_OUTPUT, NUM_OUTPUTS };
  enum LightIds { NUM_LIGHTS };

  DMXModule( ) : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
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

void DMXModule::step( ) {
  float in1 = inputs[ NOTE1_INPUT ].value;
  struct DMXContainer *note1;

  // check the first note
  note1 = getNote(in1);

  if (note1 == NULL) {
    currentStep1 = 0;
    outputs[ AUDIO1_OUTPUT ].value = 0;
    last1 = -1;
  } else {
    if (last1 != note1->current) {
      last1 = note1->current;
      currentStep1 = 0;
    }

    if (currentStep1 >= note1->length) {
      outputs[ AUDIO1_OUTPUT ].value = 0;
    } else {
      outputs[ AUDIO1_OUTPUT ].value = note1->sample[currentStep1];
      currentStep1++;
    }
  }

  float in2 = inputs[ NOTE2_INPUT ].value;
  struct DMXContainer *note2;

  // check the first note
  note2 = getNote(in2);

  if (note2 == NULL) {
    currentStep2 = 0;
    outputs[ AUDIO2_OUTPUT ].value = 0;
    last2 = -1;
  } else {
    if (last2 != note2->current) {
      last2 = note2->current;
      currentStep2 = 0;
    }

    if (currentStep2 >= note2->length) {
      outputs[ AUDIO2_OUTPUT ].value = 0;
    } else {
      outputs[ AUDIO2_OUTPUT ].value = note2->sample[currentStep2];
      currentStep2++;
    }
  }

}

struct DMXWidget : ModuleWidget {
  DMXWidget(DMXModule *module);
};

DMXWidget::DMXWidget(DMXModule *module) : ModuleWidget(module) {
  box.size = Vec(3 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  {
    SVGPanel *panel = new SVGPanel( );
    panel->box.size = box.size;
    panel->setBackground(SVG::load(assetPlugin(pluginInstance, "res/DMX.svg")));
    addChild(panel);
  }

  addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
  addChild(createWidget<ScrewBlack>(
      Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

  addInput(createPort<CDPort>(Vec(10, 30), PortWidget::INPUT, module,
                                   DMXModule::NOTE1_INPUT));
  addOutput(createPort<CDPort>(Vec(10, 120), PortWidget::OUTPUT, module,
                                     DMXModule::AUDIO1_OUTPUT));
  addInput(createPort<CDPort>(Vec(10, 220), PortWidget::INPUT, module,
                                   DMXModule::NOTE2_INPUT));
  addOutput(createPort<CDPort>(Vec(10, 310), PortWidget::OUTPUT, module,
                                     DMXModule::AUDIO2_OUTPUT));

}

Model *modelDMX = createModel<DMXModule, DMXWidget>("DMX");
