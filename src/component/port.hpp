#pragma once

#include "shadow.hpp"

struct DKPort : app::SvgPort {
private:
  DKShadow shadow = DKShadow();

public:
  DKPort() {
    setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/component/Port.svg")));

    /** inherit dimensions */
    shadow.setBox(box);
    shadow.setSize(0.2);
    shadow.setShadowPosition(2, 1);
  }

  void draw(const DrawArgs &args) override {
    shadow.draw(args.vg);
    app::SvgPort::draw(args);
  }
};
