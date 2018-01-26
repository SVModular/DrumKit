#include <stdint.h>

#include "DrumKit.hpp"
#include "../deps/SynthDevKit/src/CV.hpp"
#include "kick.h"

struct BD9Container {
  float *sample;
  unsigned int length;
};

struct BD9Container *getState (float current) {
  static struct BD9Container c;

  switch ((int) current) {
  case 1:
    c.length = kick1_len;
    c.sample = (float *) kick1;
    break;

  case 2:
    c.length = kick2_len;
    c.sample = (float *) kick2;
    break;

  case 3:
    c.length = kick3_len;
    c.sample = (float *) kick3;
    break;

  case 4:
    c.length = kick4_len;
    c.sample = (float *) kick4;
    break;

  case 5:
    c.length = kick5_len;
    c.sample = (float *) kick5;
    break;

  case 6:
    c.length = kick6_len;
    c.sample = (float *) kick6;
    break;

  case 7:
    c.length = kick7_len;
    c.sample = (float *) kick7;
    break;

  case 8:
    c.length = kick8_len;
    c.sample = (float *) kick8;
    break;

  case 9:
    c.length = kick9_len;
    c.sample = (float *) kick9;
    break;

  case 10:
    c.length = kick10_len;
    c.sample = (float *) kick10;
    break;

  case 11:
    c.length = kick11_len;
    c.sample = (float *) kick11;
    break;

  case 12:
    c.length = kick12_len;
    c.sample = (float *) kick12;
    break;

  case 13:
    c.length = kick13_len;
    c.sample = (float *) kick13;
    break;

  case 14:
    c.length = kick14_len;
    c.sample = (float *) kick14;
    break;

  case 15:
    c.length = kick15_len;
    c.sample = (float *) kick15;
    break;

  case 16:
    c.length = kick16_len;
    c.sample = (float *) kick16;
    break;
  }

  return &c;
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
  struct BD9Container *c = getState(current1);

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
  c = getState(current2);

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
