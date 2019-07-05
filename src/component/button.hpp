#pragma once

struct DKMomentaryUpButton : app::SvgSwitch {
	DKMomentaryUpButton() {
		momentary = true;
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/component/led-up-off.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/component/led-up-on.svg")));
	}
};

struct DKMomentaryDownButton : app::SvgSwitch {
	DKMomentaryDownButton() {
		momentary = true;
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/component/led-down-off.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/component/led-down-on.svg")));
	}
};
