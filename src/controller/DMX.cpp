#include "DMX.hpp"
#include "../samples/dmx.h"

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


DMXModule::DMXModule( ) {
  config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
  currentStep1 = 0;
  last1 = -1;
  currentStep2 = 0;
  last2 = -1;
}

void DMXModule::process(const ProcessArgs &args) {
  float in1 = inputs[ NOTE1_INPUT ].getVoltage();
  struct DMXContainer *note1;

  // check the first note
  note1 = getNote(in1);

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
  struct DMXContainer *note2;

  // check the first note
  note2 = getNote(in2);

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
