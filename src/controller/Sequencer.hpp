#include <cstdint>

#include "../DrumKit.hpp"

#include "../../deps/SynthDevKit/src/CV.hpp"

#define SEQ_TRACKS 8
#define SEQ_BEATS 16
#define SEQ_PLAY 8
#define MIN_PULSE_WIDTH 10
#define MAX_PULSE_WIDTH 1000
#define NUM_PATTERNS 16

struct SequencerModule : Module {
  enum ParamIds {
    PAD_PARAM,
    PLAY = SEQ_TRACKS * SEQ_BEATS,
    MAIN_UP,
    MAIN_DOWN,
    PULSE_WIDTH,
    PATTERN_UP,
    PATTERN_DOWN = PATTERN_UP + SEQ_PLAY,
    CYCLE        = PATTERN_DOWN + SEQ_PLAY,
    COPY,
    PASTE,
    NUM_PARAMS
  };
  enum InputIds {
    CLOCK,
    RUN_CV,
    CYCLE_CV,
    RESET,
    PULSE_WIDTH_CV,
    MAIN_SELECT,
    TRACK_SELECT,
    NUM_INPUTS = TRACK_SELECT + SEQ_PLAY
  };
  enum OutputIds { GATE_OUT, NUM_OUTPUTS = SEQ_TRACKS };
  enum LightIds {
    RUN_LIGHT,
    CYCLE_LIGHT,
    QTR_LED,
    BEAT_LED   = QTR_LED + (SEQ_BEATS / 4),
    PLAY_LED   = BEAT_LED + SEQ_BEATS,
    COPY_LIGHT = PLAY_LED + SEQ_PLAY,
    PASTE_LIGHT,
    NUM_LIGHTS
  };

  SequencerModule( );
  ~SequencerModule() override;

  void process(const ProcessArgs &) override;
  json_t *dataToJson() override;
  void dataFromJson(json_t *) override;

  uint16_t pulseWidth( );
  void setPlay(uint8_t);
  void alterPattern(uint8_t, int8_t, uint8_t);
  void updateCurrentPosition( );
  void savePattern(uint8_t);
  void checkPatternCV(uint8_t);
  void doReset( );
  void copyPattern();
  void pastePattern();

  SynthDevKit::CV *clock;
  SynthDevKit::CV *runCV;
  SynthDevKit::CV *runParam;
  SynthDevKit::CV *cycleCV;
  SynthDevKit::CV *cycleParam;
  SynthDevKit::CV *resetCV;

  int8_t currentCount;
  int8_t currentPlay;
  int16_t currentGate;
  uint64_t currentPulse;
  int8_t currentPosition;
  bool running = true;
  bool cycling = true;

  int8_t programs[SEQ_PLAY];

  SynthDevKit::CV *patternUp[SEQ_PLAY];
  SynthDevKit::CV *patternDown[SEQ_PLAY];
  SynthDevKit::CV *mainUp;
  SynthDevKit::CV *mainDown;
  SynthDevKit::CV *copy;
  SynthDevKit::CV *paste;

  // play * beats * tracks
  bool tracks[NUM_PATTERNS + 1][ SEQ_BEATS ][ SEQ_TRACKS ];

  // copied pattern
  bool copied[SEQ_BEATS][SEQ_TRACKS];
};
