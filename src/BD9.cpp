#include <stdint.h>

#include "DrumKit.hpp"
#include "../deps/SynthDevKit/src/CV.hpp"
#include "kick.h"

struct BD9Container {
  float *sample;
  unsigned int length;
};

struct BD9Container bd9samples[16] = {
  { (float *) kick1, kick1_len },
  { (float *) kick2, kick2_len },
  { (float *) kick3, kick3_len },
  { (float *) kick4, kick4_len },
  { (float *) kick5, kick5_len },
  { (float *) kick6, kick6_len },
  { (float *) kick7, kick7_len },
  { (float *) kick8, kick8_len },
  { (float *) kick9, kick9_len },
  { (float *) kick10, kick10_len },
  { (float *) kick11, kick11_len },
  { (float *) kick12, kick12_len },
  { (float *) kick13, kick13_len },
  { (float *) kick14, kick14_len },
  { (float *) kick15, kick15_len },
  { (float *) kick16, kick16_len },
};

struct BD9Container *bdgetState (float current) {
  return &bd9samples[(int) current - 1];
}


struct BD9Module : Module {
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

	BD9Module() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
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


void BD9Module::step() {
	float in1 = inputs[CLOCK1_INPUT].value;
  cv1->update(in1);

  if (cv1->newTrigger()) {
    if (!ready1) {
      ready1 = true;
    }

    currentStep1 = 0;
  }

  float current1 = params[DRUM1_PARAM].value;
  struct BD9Container *c = bdgetState(current1);

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
  c = bdgetState(current2);

  if (currentStep2 >= c->length) {
    outputs[AUDIO2_OUTPUT].value = 0;
  } else {
    outputs[AUDIO2_OUTPUT].value = c->sample[currentStep2];
    currentStep2++;
  }
}


BD9Widget::BD9Widget() {
	BD9Module *module = new BD9Module();
	setModule(module);
	box.size = Vec(3 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/BD9.svg")));
		addChild(panel);
	}

	addChild(createScrew<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
	addChild(createScrew<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

  addInput(createInput<PJ301MPort>(Vec(10, 45), module, BD9Module::CLOCK1_INPUT));
	addParam(createParam<RoundSmallBlackSnapKnob>(Vec(8, 92), module, BD9Module::DRUM1_PARAM, 1.0, 16.0, 9.0));

	addOutput(createOutput<PJ301MPort>(Vec(10, 149), module, BD9Module::AUDIO1_OUTPUT));

  addInput(createInput<PJ301MPort>(Vec(10, 205), module, BD9Module::CLOCK2_INPUT));
	addParam(createParam<RoundSmallBlackSnapKnob>(Vec(8, 252), module, BD9Module::DRUM2_PARAM, 1.0, 16.0, 9.0));

	addOutput(createOutput<PJ301MPort>(Vec(10, 308), module, BD9Module::AUDIO2_OUTPUT));
}
