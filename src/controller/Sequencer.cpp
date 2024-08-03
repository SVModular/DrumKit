#include "Sequencer.hpp"
#include "../component/tooltips.hpp"

static uint8_t patternForVoltage(float voltage) {
  float divisions = (float) 10 / (float) (NUM_PATTERNS + 1);

  int8_t pattern = (int8_t) (voltage / divisions);
  if (pattern < 0) {
    pattern = 0;
  } else if (pattern > NUM_PATTERNS) {
    pattern = NUM_PATTERNS;
  }

  return (uint8_t) pattern;
}

SequencerModule::SequencerModule() {
  config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

  clock = new SynthDevKit::CV(0.5);
  resetCV = new SynthDevKit::CV(0.5);
  runParam = new SynthDevKit::CV(0.5);
  runCV = new SynthDevKit::CV(0.5);
  cycleParam = new SynthDevKit::CV(0.5);
  cycleCV = new SynthDevKit::CV(0.5);
  mainUp = new SynthDevKit::CV(0.5);
  mainDown = new SynthDevKit::CV(0.5);
  copy = new SynthDevKit::CV(0.5);
  paste = new SynthDevKit::CV(0.5);

  for (int i = 0; i < SEQ_PLAY; i++) {
    patternUp[i] = new SynthDevKit::CV(0.5);
    patternDown[i] = new SynthDevKit::CV(0.5);

    // set up the programs for default, first is "1", the rest are 0
    programs[i] = 0;

    configParam<Blank>(PATTERN_UP + i, 0.0, 1.0, 0.0, "Up");
    configParam<Blank>(PATTERN_DOWN + i, 0.0, 1.0, 0.0, "Down");
  }
  programs[0] = 1;

  for (int i = 0; i < SEQ_TRACKS; i++) {
    // sequence pads
    for (int j = 0; j < SEQ_BEATS; j++) {
      configParam<Toggle>(PAD_PARAM + j + (i * SEQ_BEATS), 0.0, 1.0, 0.0, std::to_string(j + 1) + "/" + std::to_string(i + 1));
    }
  }

  for (int i = 0; i <= NUM_PATTERNS; i++) {
    // reset the tracks to empty
    for (int j = 0; j < SEQ_BEATS; j++) {
      for (int k = 0; k < SEQ_TRACKS; k++) {
        tracks[i][j][k] = false;
      }
    }
  }

  configParam<PercentTen>(PULSE_WIDTH, 0.1, 10.0, 5.05, "Width", "%");
  configParam<Blank>(PLAY, 0.0f, 1.0f, 0.0f, "Run");
  configParam<Blank>(CYCLE, 0.0f, 1.0f, 0.0f, "Cycle");
  configParam<Blank>(MAIN_UP, 0.0f, 1.0f, 0.0f, "Up");
  configParam<Blank>(MAIN_DOWN, 0.0f, 1.0f, 0.0f, "Down");
  configParam<Blank>(COPY, 0.0f, 1.0f, 0.0f, "Copy");
  configParam<Blank>(PASTE, 0.0f, 1.0f, 0.0f, "Paste");

  doReset();
}

SequencerModule::~SequencerModule() {
  delete clock;
  delete runCV;
  delete runParam;
  delete cycleCV;
  delete cycleParam;
  delete resetCV;
  delete mainUp;
  delete mainDown;
  delete copy;
  delete paste;
  for (int i = 0; i < SEQ_PLAY; i++) {
    delete patternUp[i];
    delete patternDown[i];
  }
}

void SequencerModule::doReset() {
  currentCount = -1;
  currentGate = -1;
  currentPlay = 1;
  currentPosition = 0;
  savePattern(currentPlay);
  setPlay(programs[0]);
}

json_t *SequencerModule::dataToJson() {
  savePattern(currentPlay);

  json_t *rootJ = json_object();
  json_object_set_new(rootJ, "running", json_boolean(running));
  json_object_set_new(rootJ, "cycling", json_boolean(cycling));
  json_object_set_new(rootJ, "currentPlay", json_integer(currentPlay));

  json_t *j_patterns = json_array();
  for (uint8_t i = 0; i <= NUM_PATTERNS; i++) {
    json_t *j_beats = json_array();

    for (uint8_t j = 0; j < SEQ_BEATS; j++) {
      json_t *j_tracks = json_array();

      for (uint8_t k = 0; k < SEQ_TRACKS; k++) {
        json_array_append(j_tracks, json_boolean(tracks[i][j][k]));
      }

      json_array_append(j_beats, j_tracks);
    }

    json_array_append(j_patterns, j_beats);
  }

  json_object_set_new(rootJ, "tracks", j_patterns);

  json_t *j_programs = json_array();

  for (uint8_t i = 0; i < SEQ_PLAY; i++) {
    json_array_append(j_programs, json_integer(programs[i]));
  }

  json_object_set_new(rootJ, "programs", j_programs);

  return rootJ;
}

void SequencerModule::dataFromJson(json_t *rootJ) {
  json_t *r = json_object_get(rootJ, "running");
  running = json_boolean_value(r);

  json_t *c = json_object_get(rootJ, "cycling");
  cycling = json_boolean_value(c);

  json_t *p = json_object_get(rootJ, "currentPlay");
  currentPlay = json_integer_value(p);
  if (currentPlay > NUM_PATTERNS || currentPlay < 1) {
    currentPlay = 1;
  }

  json_t *cp = json_object_get(rootJ, "currentPosition");
  currentPosition = json_integer_value(cp);
  if (currentPosition < 0 || currentPosition >= SEQ_TRACKS) {
    currentPosition = 0;
  }

  json_t *j_programs = json_object_get(rootJ, "programs");

  for (uint8_t i = 0; i < json_array_size(j_programs) && i < SEQ_PLAY; i++) {
    programs[i] = json_integer_value(json_array_get(j_programs, i));
  }

  json_t *j_patterns = json_object_get(rootJ, "tracks");
  for (uint8_t i = 0; i < json_array_size(j_patterns) && i <= NUM_PATTERNS; i++) {
    json_t *j_beats = json_array_get(j_patterns, i);

    for (uint8_t j = 0; j < json_array_size(j_beats) && j < SEQ_BEATS; j++) {
      json_t *j_tracks = json_array_get(j_beats, j);

      for (uint8_t k = 0; k < json_array_size(j_tracks) && k < SEQ_TRACKS; k++) {
        tracks[i][j][k] = json_boolean_value(json_array_get(j_tracks, k));
      }
    }
  }
}

uint16_t SequencerModule::pulseWidth ( ) {
  float pw = (inputs[PULSE_WIDTH_CV].isConnected() ? inputs[PULSE_WIDTH_CV].getVoltage() : 0 ) + params[PULSE_WIDTH].getValue();
  pw = clamp((pw * MAX_PULSE_WIDTH) / 10, (float) MIN_PULSE_WIDTH, (float) MAX_PULSE_WIDTH);

  return (uint16_t) pw;
}

void SequencerModule::setPlay(uint8_t which) {
  for (int i = 0; i < SEQ_BEATS; i++) {
    for (int j = 0; j < SEQ_TRACKS; j++) {
      params[PAD_PARAM + (SEQ_TRACKS * i) + j].setValue(tracks[which][i][j]);
    }
  }

  currentPlay = which;
}

void SequencerModule::savePattern(uint8_t which) {
  for (int i = 0; i < SEQ_BEATS; i++) {
    for (int j = 0; j < SEQ_TRACKS; j++) {
      tracks[which][i][j] = params[PAD_PARAM + (SEQ_TRACKS * i) + j].getValue();
    }
  }
}

void SequencerModule::pastePattern() {
  for (int i = 0; i < SEQ_BEATS; i++) {
    for (int j = 0; j < SEQ_TRACKS; j++) {
      params[PAD_PARAM + (SEQ_TRACKS * i) + j].setValue(copied[i][j]);
    }
  }

  savePattern(currentPlay);
}

void SequencerModule::copyPattern() {
  for (int i = 0; i < SEQ_BEATS; i++) {
    for (int j = 0; j < SEQ_TRACKS; j++) {
      copied[i][j] = params[PAD_PARAM + (SEQ_TRACKS * i) + j].getValue();
    }
  }
}

void SequencerModule::updateCurrentPosition() {
  if (!cycling) {
    return;
  }

  // since the current position can change with the large selector, use current play
  savePattern(currentPlay);

  do {
    currentPosition++;
  }
  while(currentPosition < SEQ_PLAY && programs[currentPosition] == 0);

  if (currentPosition == SEQ_PLAY) {
    currentPosition = 0;
  }

  setPlay(programs[currentPosition]);
}

void SequencerModule::checkPatternCV(uint8_t which) {
  if (inputs[TRACK_SELECT + which].isConnected()) {
    programs[which] = patternForVoltage(inputs[TRACK_SELECT + which].getVoltage());
  }

  if (which == 0 && programs[which] == 0) {
    programs[0] = 1;
  }
}

void SequencerModule::alterPattern(uint8_t which, int8_t amount, uint8_t to = 0) {
  if (amount == 0) {
    programs[which] = to;
  } else {
    programs[which] += amount;
    if (programs[which] > NUM_PATTERNS) {
      programs[which] = NUM_PATTERNS;
    } else if (programs[which] < 0) {
      programs[which] = 0;
    }

    // program 0 should never be 0, 1 or more
    if (which == 0) {
      if (programs[which] == 0) {
        programs[which] = 1;
      }
    }
  }
}

void SequencerModule::process(const ProcessArgs &args) {
  for (uint8_t i = 0; i < SEQ_PLAY; i++) {
    patternUp[i]->update(params[PATTERN_UP + i].getValue());
    patternDown[i]->update(params[PATTERN_DOWN + i].getValue());

    if (patternUp[i]->newTrigger()) {
      alterPattern(i, 1);
    }

    if (patternDown[i]->newTrigger()) {
      alterPattern(i, -1);
    }
  }

  // main up and down
  mainUp->update(params[MAIN_UP].getValue());
  mainDown->update(params[MAIN_DOWN].getValue());

  if (mainDown->newTrigger()) {
    if (currentPlay > 1) {
      savePattern(currentPlay);
      setPlay(currentPlay - 1);
    }
  }

  if (mainUp->newTrigger()) {
    if (currentPlay < NUM_PATTERNS) {
      savePattern(currentPlay);
      setPlay(currentPlay + 1);
    }
  }

  // copy
  copy->update(params[COPY].getValue());
  if (copy->newTrigger()) {
    copyPattern();
  }
  lights[COPY_LIGHT].value = params[COPY].getValue() ? 1.0f : 0.0f;

  // paste
  paste->update(params[PASTE].getValue());
  if (paste->newTrigger()) {
    pastePattern();
  }
  lights[PASTE_LIGHT].value = params[PASTE].getValue() ? 1.0f : 0.0f;

  // reset
  if (inputs[RESET].isConnected()) {
    resetCV->update(inputs[RESET].getVoltage());
    if (resetCV->newTrigger()) {
      doReset();
    }
  }

  // run/stop
  if (inputs[RUN_CV].isConnected()) {
    runCV->update(inputs[RUN_CV].getVoltage());
    if (runCV->newTrigger()) {
      running = !running;
    }
  }

  runParam->update(params[PLAY].getValue());
  if (runParam->newTrigger()) {
    running = !running;
  }

  lights[RUN_LIGHT].value = running ? 1.0f : 0.0f;

  // cycling
  if (inputs[CYCLE_CV].isConnected()) {
    cycleCV->update(inputs[CYCLE_CV].getVoltage());
    if (cycleCV->newTrigger()) {
      cycling = !cycling;
    }
  }

  cycleParam->update(params[CYCLE].getValue());
  if (cycleParam->newTrigger()) {
    cycling = !cycling;
  }

  lights[CYCLE_LIGHT].value = cycling ? 1.0f : 0.0f;

  if (!running) {
    return;
  }


  clock->update(inputs[CLOCK].isConnected() ? inputs[CLOCK].getVoltage() : 0.0f);

  if (clock->newTrigger()) {
    currentCount++;
    currentPulse = 0;

    for (int i = 0; i < SEQ_BEATS; i++) {
      lights[BEAT_LED + i].value = (i == currentCount) ? 1.0f : 0.0f;
    }

    int qtr = (int) (currentCount / 4);
    for (int i = 0; i < SEQ_BEATS / 4; i++) {
      lights[QTR_LED + i].value = (i == qtr) ? 1.0f : 0;
    }
  }

  // pattern cv selection
  for (int i = 0; i < SEQ_PLAY; i++) {
    checkPatternCV(i);
    lights[PLAY_LED + i].value = (i == currentPosition) ? 1.0f : 0;
  }

  // main pattern cv selection
  if (inputs[MAIN_SELECT].isConnected()) {
    savePattern(currentPlay);
    uint8_t pattern = patternForVoltage(inputs[MAIN_SELECT].getVoltage());
    if (pattern == 0) {
      pattern = 1;
    }
    setPlay(pattern);
  }

  // check to see if this is an active beat
  if (currentCount != -1) {
    uint16_t pw = pulseWidth();

    for (int i = 0; i < SEQ_TRACKS; i++) {
      outputs[GATE_OUT + i].setVoltage(0);

      if (params[PAD_PARAM + ((SEQ_BEATS * i) + currentCount)].getValue()) {
        if (currentPulse < pw) {
          outputs[GATE_OUT + i].setVoltage(5);
        }
      }
    }
  } else {
    for (int i = 0; i < SEQ_TRACKS; i++) {
      outputs[GATE_OUT + i].setVoltage(0);
    }
  }

  if (currentCount == SEQ_BEATS - 1) {
    currentCount = -1;

    updateCurrentPosition();
  }


  currentPulse++;
}
