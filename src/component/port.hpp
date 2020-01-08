#pragma once

#include "shadow.hpp"

struct LocalPort : PortWidget {
	widget::FramebufferWidget *fb;
	widget::SvgWidget *sw;

  LocalPort() {
  	fb = new widget::FramebufferWidget;
  	addChild(fb);

  	sw = new widget::SvgWidget;
  	fb->addChild(sw);
  }

  void setSvg(std::shared_ptr<Svg> svg) {
  	sw->setSvg(svg);
  	fb->box.size = sw->box.size;
  	box.size = sw->box.size;

  	fb->dirty = true;
  }

};

struct DKPort : LocalPort {
private:
  DKShadow shadow = DKShadow();

public:
  DKPort() {
    setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/component/Port.svg")));

    /** inherit dimensions */
    shadow.setBox(box);
    shadow.setSize(0.4);
    shadow.setStrength(0.2);
    shadow.setShadowPosition(2, 3.5);
  }

  void draw(const DrawArgs &args) override {
    shadow.draw(args.vg);
    PortWidget::draw(args);
  }
};
