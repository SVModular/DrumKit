#pragma once


struct DKSwitch : app::SvgSwitch {
	DKSwitch() {
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/component/switch_0.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/component/switch_1.svg")));
	}
};
