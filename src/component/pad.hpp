#pragma once


struct DKPad : app::SvgSwitch {
	DKPad() {
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/component/pad-off.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/component/pad-on.svg")));
	}
};

struct DKMomentaryPad : app::SvgSwitch {
	DKMomentaryPad() {
		momentary = true;
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/component/pad-off.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/component/pad-on.svg")));
	}
};
