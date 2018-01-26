#include <stdint.h>

#include "DrumKit.hpp"
#include "../deps/SynthDevKit/src/CV.hpp"
#include "snare.h"

struct SnareContainer {
  float *sample;
  unsigned int length;
};

struct SnareContainer snsamples[16] = {
  { (float *) snare1, snare1_len },
  { (float *) snare2, snare2_len },
  { (float *) snare3, snare3_len },
  { (float *) snare4, snare4_len },
  { (float *) snare5, snare5_len },
  { (float *) snare6, snare6_len },
  { (float *) snare7, snare7_len },
  { (float *) snare8, snare8_len },
  { (float *) snare9, snare9_len },
  { (float *) snare10, snare10_len },
  { (float *) snare11, snare11_len },
  { (float *) snare12, snare12_len },
  { (float *) snare13, snare13_len },
  { (float *) snare14, snare14_len },
  { (float *) snare15, snare15_len },
  { (float *) snare16, snare16_len },
};

struct SnareContainer *sngetState (float current) {
  return &snsamples[(int) current - 1];
}


struct SnareModule : Module {
	enum ParamIds {
    DRUM1_PARAM,
    DRUM2_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		CLOCK1_INPUT,
    CLOCK2_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		AUDIO1_OUTPUT,
    AUDIO2_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	SnareModule() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
    cv1 = new SynthDevKit::CV(1.7f);
    cv2 = new SynthDevKit::CV(1.7f);
    currentStep1 = 0;
    currentStep2 = 0;
    ready1 = false;
    ready2 = false;
  }

	void step() override;

  SynthDevKit::CV *cv1;
  uint32_t currentStep1;
  bool ready1;
  SynthDevKit::CV *cv2;
  uint32_t currentStep2;
  bool ready2;
};


void SnareModule::step() {
	float in1 = inputs[CLOCK1_INPUT].value;
  cv1->update(in1);

  if (cv1->newTrigger()) {
    if (!ready1) {
      ready1 = true;
    }

    currentStep1 = 0;
  }

  float current1 = params[DRUM1_PARAM].value;
  struct SnareContainer *c = sngetState(current1);

  if (currentStep1 >= c->length) {
    outputs[AUDIO1_OUTPUT].value = 0;
  } else {
    outputs[AUDIO1_OUTPUT].value = c->sample[currentStep1];
    currentStep1++;
  }

  float in2 = inputs[CLOCK2_INPUT].value;
  cv2->update(in2);

  if (cv2->newTrigger()) {
    if (!ready2) {
      ready2 = true;
    }

    currentStep2 = 0;
  }

  float current2 = params[DRUM2_PARAM].value;
  c = sngetState(current2);

  if (currentStep2 >= c->length) {
    outputs[AUDIO2_OUTPUT].value = 0;
  } else {
    outputs[AUDIO2_OUTPUT].value = c->sample[currentStep2];
    currentStep2++;
  }
}


SnareWidget::SnareWidget() {
	SnareModule *module = new SnareModule();
	setModule(module);
	box.size = Vec(3 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/Snare.svg")));
		addChild(panel);
	}

	addChild(createScrew<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
	addChild(createScrew<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

  addInput(createInput<PJ301MPort>(Vec(10, 45), module, SnareModule::CLOCK1_INPUT));
	addParam(createParam<RoundSmallBlackSnapKnob>(Vec(8, 92), module, SnareModule::DRUM1_PARAM, 1.0, 16.0, 9.0));

	addOutput(createOutput<PJ301MPort>(Vec(10, 149), module, SnareModule::AUDIO1_OUTPUT));

  addInput(createInput<PJ301MPort>(Vec(10, 205), module, SnareModule::CLOCK2_INPUT));
	addParam(createParam<RoundSmallBlackSnapKnob>(Vec(8, 252), module, SnareModule::DRUM2_PARAM, 1.0, 16.0, 9.0));

	addOutput(createOutput<PJ301MPort>(Vec(10, 308), module, SnareModule::AUDIO2_OUTPUT));
}
