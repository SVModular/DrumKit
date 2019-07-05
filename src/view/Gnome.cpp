#include "../controller/Gnome.hpp"
#include "../component/port.hpp"
#include "../component/knob.hpp"
#include "../component/display.hpp"
#include "../component/light.hpp"
#include "../component/switch.hpp"

struct GnomeWidget : ModuleWidget {
  GnomeWidget(GnomeModule *module);
};

GnomeWidget::GnomeWidget(GnomeModule *module) {
	setModule(module);
  box.size = Vec(9 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Gnome.svg")));

  addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
  addChild(createWidget<ScrewBlack>(
      Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

  {
    BPMDisplay *bpm = new BPMDisplay();
    if (module) {
      bpm->value = &module->bpm;
    }
    bpm->box.pos = Vec(71, 102);
    bpm->box.size = Vec(70, 45);
    addChild(bpm);
  }

  {
    WidthDisplay *width = new WidthDisplay();
    if (module) {
      width->value = &module->width;
    }
    width->box.pos = Vec(60, 125);
    width->box.size = Vec(80, 45);
    addChild(width);
  }

  addParam(createParam<DKSwitch>(Vec(83, 39), module, GnomeModule::SWING));

  addParam(createParam<DKLEDBezel>(Vec(28, 33), module,
                                         GnomeModule::RUN));
  addChild(createLight<DKButtonLight<GreenLight>>(
      Vec(30, 35), module, GnomeModule::RUN_LIGHT));
  addInput(createInput<DKPort>(Vec(4, 33), module,
                                   GnomeModule::RUN_CV));

  addParam(createParam<LightKnobFull>(
      Vec(28.5, 72.5), module, GnomeModule::BPM));
  addInput(createInput<DKPort>(Vec(4, 78), module,
                                   GnomeModule::BPM_CV));

  addParam(createParam<LightKnobFull>(
      Vec(28.5, 117.5), module, GnomeModule::WIDTH));
  addInput(createInput<DKPort>(Vec(4, 123), module,
                                   GnomeModule::WIDTH_CV));

  addInput(createInput<DKPort>(Vec(10, 165), module,
                                   GnomeModule::RESET));
  addOutput(createOutput<DKPort>(Vec(55, 165), module,
                                     GnomeModule::NOTE_1));
  addChild(createLight<SmallLight<GreenLight>>(
      Vec(75, 185), module, GnomeModule::LED_1));

  addOutput(createOutput<DKPort>(Vec(100, 165), module,
                                   GnomeModule::NOTE_2));
  addChild(createLight<SmallLight<GreenLight>>(
      Vec(120, 185), module, GnomeModule::LED_2));

  addOutput(createOutput<DKPort>(Vec(10, 210), module,
                                   GnomeModule::NOTE_4));
  addChild(createLight<SmallLight<GreenLight>>(
      Vec(30, 230), module, GnomeModule::LED_4));

  addOutput(createOutput<DKPort>(Vec(55, 210), module,
                                     GnomeModule::NOTE_8));
  addChild(createLight<SmallLight<GreenLight>>(
      Vec(75, 230), module, GnomeModule::LED_8));

  addOutput(createOutput<DKPort>(Vec(100, 210), module,
                                   GnomeModule::NOTE_16));
  addChild(createLight<SmallLight<GreenLight>>(
      Vec(120, 230), module, GnomeModule::LED_16));

  addOutput(createOutput<DKPort>(Vec(10, 255), module,
                                   GnomeModule::NOTE_A));
  addChild(createLight<SmallLight<GreenLight>>(
      Vec(30, 275), module, GnomeModule::LED_A));

  addOutput(createOutput<DKPort>(Vec(55, 255), module,
                                     GnomeModule::NOTE_B));
  addChild(createLight<SmallLight<GreenLight>>(
      Vec(75, 275), module, GnomeModule::LED_B));

  addOutput(createOutput<DKPort>(Vec(100, 255), module,
                                   GnomeModule::NOTE_C));
  addChild(createLight<SmallLight<GreenLight>>(
      Vec(120, 275), module, GnomeModule::LED_C));

  addOutput(createOutput<DKPort>(Vec(10, 300), module,
                                   GnomeModule::NOTE_D));
  addChild(createLight<SmallLight<GreenLight>>(
      Vec(30, 320), module, GnomeModule::LED_D));

  addOutput(createOutput<DKPort>(Vec(55, 300), module,
                                     GnomeModule::NOTE_E));
  addChild(createLight<SmallLight<GreenLight>>(
      Vec(75, 320), module, GnomeModule::LED_E));

  addOutput(createOutput<DKPort>(Vec(100, 300), module,
                                   GnomeModule::NOTE_F));
  addChild(createLight<SmallLight<GreenLight>>(
      Vec(120, 320), module, GnomeModule::LED_F));
}

Model *modelGnome = createModel<GnomeModule, GnomeWidget>("Gnome");
