#include "../controller/Sequencer.hpp"

#include "../component/pad.hpp"
#include "../component/knob.hpp"
#include "../component/port.hpp"
#include "../component/display.hpp"
#include "../component/button.hpp"
#include "../component/light.hpp"

struct SequencerWidget : ModuleWidget {
  SequencerWidget(SequencerModule *module);
};

SequencerWidget::SequencerWidget(SequencerModule *module) {
  setModule(module);
  box.size = Vec(32 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Sequencer.svg")));

  {
    TrackDisplay *track = new TrackDisplay();
    if (module) {
      track->value = &module->currentPlay;
    }
    track->box.pos = Vec(197, 80);
    track->box.size = Vec(70, 45);
    addChild(track);
  }

  addParam(createParam<DKMomentaryUpButton>(
        Vec(190, 94), module, SequencerModule::MAIN_UP));
  addParam(createParam<DKMomentaryDownButton>(
        Vec(216, 94), module, SequencerModule::MAIN_DOWN));

  for (int i = 0; i < SEQ_TRACKS; i++) {
    // sequence pads
    for (int j = 0; j < SEQ_BEATS; j++) {
      addParam(createParam<DKPad>(
            Vec(46 + (j * 25), 151 + (i * 25)), module, SequencerModule::PAD_PARAM + j + (i * SEQ_BEATS)));
    }

    // gate out
    addOutput(createOutput<DKPort>(Vec(449.5, 150 + (25 * i)), module,
                                   SequencerModule::GATE_OUT + i));
  }

  // beat led's
  for (int i = 0; i < SEQ_BEATS; i++) {
    addChild(createLight<TinyLight<GreenLight>>(
             Vec(56 + (i * 25), 354), module, SequencerModule::BEAT_LED + i));
  }


  // qtr led's
  for (int i = 0; i < SEQ_BEATS / 4; i++) {
    addChild(createLight<TinyLight<GreenLight>>(
             Vec(56 + (i * 100), 143), module, SequencerModule::QTR_LED + i));

  }

  // pulse width
  addParam(createParam<LightKnobFull>(
      Vec(118.5, 72.5), module, SequencerModule::PULSE_WIDTH));
  addInput(createInput<DKPort>(Vec(94, 78), module,
                                   SequencerModule::PULSE_WIDTH_CV));


  // clock in
  addInput(createInput<DKPort>(Vec(10, 30), module,
                                 SequencerModule::CLOCK));

  // reset in
  addInput(createInput<DKPort>(Vec(10, 75), module,
                                 SequencerModule::RESET));

  // run
  addParam(createParam<DKLEDBezel>(Vec(73, 33), module,
                                         SequencerModule::PLAY));
  addChild(createLight<DKButtonLight<GreenLight>>(
      Vec(75, 35), module, SequencerModule::RUN_LIGHT));
  addInput(createInput<DKPort>(Vec(49, 30), module,
                                   SequencerModule::RUN_CV));

  // cycle
  addParam(createParam<DKLEDBezel>(Vec(73, 78), module,
                                         SequencerModule::CYCLE));
  addChild(createLight<DKButtonLight<GreenLight>>(
      Vec(75, 80), module, SequencerModule::CYCLE_LIGHT));
  addInput(createInput<DKPort>(Vec(49, 75), module,
                                   SequencerModule::CYCLE_CV));

  for (int i = 0; i < SEQ_PLAY; i++) {
    addParam(createParam<DKMomentaryUpButton>(
          Vec(248 + (i * 25), 48), module, SequencerModule::PATTERN_UP + i));
    addParam(createParam<DKMomentaryDownButton>(
          Vec(248 + (i * 25), 94), module, SequencerModule::PATTERN_DOWN + i));
    addChild(createLight<TinyLight<GreenLight>>(
             Vec(256 + (i * 25), 120), module, SequencerModule::PLAY_LED + i));
    addInput(createInput<DKPort>(Vec(245 + (i * 25), 24.5), module, SequencerModule::TRACK_SELECT + i));


    {
      SmallTrackDisplay *track = new SmallTrackDisplay();
      if (module) {
        track->value = &module->programs[i];
      }
      track->box.pos = Vec(249 + (i * 25), 86);
      track->box.size = Vec(23, 36);
      addChild(track);
    }
  }

}

Model *modelSequencer = createModel<SequencerModule, SequencerWidget>("Sequencer");
