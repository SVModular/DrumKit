#pragma once

#include "shadow.hpp"

struct DKKnob : app::SvgKnob {
protected:
  DKShadow shadow = DKShadow();

public:
  DKKnob() {
		minAngle = -0.83*M_PI;
		maxAngle = 0.83*M_PI;
	}

  void setSvg(std::shared_ptr<Svg> svg) {
    app::SvgKnob::setSvg(svg);

    shadow.setBox(box);
  }

  void draw(const DrawArgs &args) override {
    /** shadow */
    shadow.draw(args.vg);

    /** component */
    app::SvgKnob::draw(args);
  }
};

struct LightKnob : DKKnob {
  LightKnob() {
    minAngle = -0.65*M_PI;
    maxAngle = 0.65*M_PI;

    setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/component/Knob.svg")));
  }
};

struct LightKnobFull : DKKnob {
  LightKnobFull() {
    minAngle = -0.8*M_PI;
    maxAngle = 0.8*M_PI;

    setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/component/Knob.svg")));
  }
};

struct LightKnobSnap : LightKnob {
  LightKnobSnap() {
    snap = true;
  }
};

struct LightKnobSmall : DKKnob {
  LightKnobSmall() {
    setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/component/KnobSmall.svg")));
  }
};
