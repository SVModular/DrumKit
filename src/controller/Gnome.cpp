#include "Gnome.hpp"
#include "../component/tooltips.hpp"

GnomeModule::GnomeModule () {
  config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

  configParam(BPM, 30.0f, 200.0f, 115.0f, "Speed", " Beats per Minute");
  configParam<Percent>(WIDTH, 0.1f, 0.6f, 0.35, "Width");
  configParam<Blank>(RUN, 0.0f, 1.0f, 0.0f, "Run");
  configParam<Swing>(SWING, 0.0f, 1.0f, 0.0f, "Swing Note");

  reset = new SynthDevKit::CV(0.5f);
  runCV = new SynthDevKit::CV(0.5f);
  runParam = new SynthDevKit::CV(0.5f);

  doReset();
}

GnomeModule::~GnomeModule() {
  delete reset;
  delete runCV;
  delete runParam;
}

void GnomeModule::doReset() {
  bpm = 0;
  width = 0;
  count = 0;
  tickCount = 0;
  running = true;
}

json_t *GnomeModule::dataToJson() {
  json_t *rootJ = json_object();
  json_object_set_new(rootJ, "running", json_boolean(running));

  return rootJ;
}

void GnomeModule::dataFromJson(json_t *rootJ) {
  json_t *r = json_object_get(rootJ, "running");
  if (json_is_boolean(r)) {
    running = json_boolean_value(r);
  }
}

float GnomeModule::paramValue (uint16_t param, uint16_t input, float low, float high) {
  float current = params[param].getValue();

  if (inputs[input].isConnected()) {
    // high - low, divided by one tenth input voltage, plus the current value
    current += ((inputs[input].getVoltage() / 10) * (high - low));
  }

  return clamp(current, low, high);
}

void GnomeModule::toggle(uint8_t which, bool on) {
  lights[which].value = on ? 1.0f : 0.0f;
  outputs[which].setVoltage(on ? 5.0f : 0.0f);
}

void GnomeModule::process(const ProcessArgs &args) {
  // round BPM to the nearest 1/10
  bpm = (float) ((uint16_t) paramValue(BPM, BPM_CV, 30, 200));
  width = paramValue(WIDTH, WIDTH_CV, 0.1, 0.6);

  // figure out if we should reset
  reset->update(inputs[RESET].getVoltage());
  if (reset->newTrigger()) {
    doReset();
  }

  if (inputs[RUN_CV].isConnected()) {
    runCV->update(inputs[RUN_CV].getVoltage());
    if (runCV->newTrigger()) {
      running = !running;
    }
  }

  runParam->update(params[RUN].getValue());
  if (runParam->newTrigger()) {
    running = !running;
  }

  lights[RUN_LIGHT].value = running ? 1.0f : 0.0f;

  if (!running) {
    return;
  }

  double ticks = ticksPerBPM(args.sampleRate);
  double tickWidth = (ticks / 16) * width;

  // rate at which swing occurs, 8th or 16th
  uint8_t swingRate = (params[SWING].getValue() == 0) ? 2 : 1;

  // whole through 16th can toggle off when hit
  if (tickCount >= (uint32_t) tickWidth) {
    for (uint8_t i = NOTE_1; i <= NOTE_16; i++) {
      toggle(i, false);
    }
  } else {
    // otherwise turn on what is supposed to be on
    switch (count) {
      case 0:
      toggle(NOTE_1, true);
      toggle(NOTE_2, true);
      toggle(NOTE_4, true);
      toggle(NOTE_8, true);
      toggle(NOTE_16, true);
      break;

      case 1:
      case 3:
      case 5:
      case 7:
      case 9:
      case 11:
      case 13:
      case 15:
      toggle(NOTE_16, true);
      break;

      case 2:
      case 6:
      case 10:
      case 14:
      toggle(NOTE_8, true);
      toggle(NOTE_16, true);
      break;

      case 4:
      case 12:
      toggle(NOTE_16, true);
      toggle(NOTE_8, true);
      toggle(NOTE_4, true);
      break;

      case 8:
      toggle(NOTE_16, true);
      toggle(NOTE_8, true);
      toggle(NOTE_4, true);
      toggle(NOTE_2, true);
      break;
    }
  }

  // iterate through the swing outputs
  if ((count % swingRate) == 0) {
    for (uint8_t i = NOTE_A, j = 0; i <= NOTE_F; i++, j++) {
      if (count % (swingRate * 2)) {
        // odd values, offset
        if (tickCount >= (swingTimings[j] * (ticks / 16)) && tickCount <= (swingTimings[j] * (ticks / 16)) + tickWidth) {
          toggle(i, true);
        } else {
          toggle(i, false);
        }
      } else {
        // even values, on the beat
        if (tickCount < tickWidth) {
          toggle(i, true);
        } else {
          toggle(i, false);
        }
      }
    }
  }

  tickCount++;

  if (tickCount >= (ticks / 16)) {
    tickCount = 0;
    count++;

    if (count >= 16) {
      count = 0;
    }
  }
}

double GnomeModule::ticksPerBPM(uint32_t sampleRate) {
  return (sampleRate * 60) / (bpm / 4);
}
