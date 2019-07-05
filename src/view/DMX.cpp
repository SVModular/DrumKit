#include "../controller/DMX.hpp"
#include "../component/port.hpp"
#include "../component/knob.hpp"

struct DMXWidget : ModuleWidget {
  DMXWidget(DMXModule *module);
};

DMXWidget::DMXWidget(DMXModule *module) {
	setModule(module);
  box.size = Vec(3 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/DMX.svg")));

  addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
  addChild(createWidget<ScrewBlack>(
      Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

  addInput(createInput<DKPort>(Vec(10, 30), module,
                                   DMXModule::NOTE1_INPUT));
  addOutput(createOutput<DKPort>(Vec(10, 120), module,
                                     DMXModule::AUDIO1_OUTPUT));
  addInput(createInput<DKPort>(Vec(10, 220), module,
                                   DMXModule::NOTE2_INPUT));
  addOutput(createOutput<DKPort>(Vec(10, 310), module,
                                     DMXModule::AUDIO2_OUTPUT));

}

Model *modelDMX = createModel<DMXModule, DMXWidget>("DMX");
