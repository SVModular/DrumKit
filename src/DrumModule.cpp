#include <stdint.h>

#include "../deps/SynthDevKit/src/CV.hpp"
#include "DrumKit.hpp"
#include "DrumModule.hpp"


  DrumModule::DrumModule( ) {
    cv1          = new SynthDevKit::CV(1.7f);
    cv2          = new SynthDevKit::CV(1.7f);
    currentStep1 = 0;
    currentStep2 = 0;
    ready1       = false;
    ready2       = false;
    numSamples = 0;
  }


struct DrumContainer *DrumModule::getSample(float current) {
  if (numSamples == 0) {
    setupSamples();
  }
  if (current < 1 || current >= numSamples) {
    return &samples[ 0 ];
  }
  return &samples[ (int)current - 1 ];
}

void DrumModule::process(const ProcessArgs &args) {
  float in1 = inputs[ CLOCK1_INPUT ].getVoltage();
  cv1->update(in1);

  if (cv1->newTrigger( )) {
    if (!ready1) {
      ready1 = true;
    }

    currentStep1 = 0;
  }

  float current1           = params[ DRUM1_PARAM ].getValue();
  struct DrumContainer *c = getSample(current1);

  if (currentStep1 >= c->length) {
    outputs[ AUDIO1_OUTPUT ].setVoltage(0);
  } else {
    outputs[ AUDIO1_OUTPUT ].setVoltage(c->sample[ currentStep1 ]);
    currentStep1++;
  }

  float in2 = inputs[ CLOCK2_INPUT ].getVoltage();
  cv2->update(in2);

  if (cv2->newTrigger( )) {
    if (!ready2) {
      ready2 = true;
    }

    currentStep2 = 0;
  }

  float current2 = params[ DRUM2_PARAM ].getValue();
  c              = getSample(current2);

  if (currentStep2 >= c->length) {
    outputs[ AUDIO2_OUTPUT ].setVoltage(0);
  } else {
    outputs[ AUDIO2_OUTPUT ].setVoltage(c->sample[ currentStep2 ]);
    currentStep2++;
  }
}

void DrumModule::setupSamples( ) { }
