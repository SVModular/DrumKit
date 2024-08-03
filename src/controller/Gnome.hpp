#include <cstdint>

#include "../DrumKit.hpp"
#include "../../deps/SynthDevKit/src/CV.hpp"

struct GnomeModule : Module {
  enum ParamIds { BPM, WIDTH, RUN, SWING, NUM_PARAMS };
  enum InputIds { RESET, BPM_CV, WIDTH_CV, RUN_CV, NUM_INPUTS };
  enum OutputIds { NOTE_1, NOTE_2, NOTE_4, NOTE_8, NOTE_16, NOTE_A, NOTE_B, NOTE_C, NOTE_D, NOTE_E, NOTE_F, NUM_OUTPUTS };
  enum LightIds { LED_1, LED_2, LED_4, LED_8, LED_16, LED_A, LED_B, LED_C, LED_D, LED_E, LED_F, LED_RESET, RUN_LIGHT, NUM_LIGHTS };

  GnomeModule( );
  ~GnomeModule();
  float paramValue (uint16_t, uint16_t, float, float);
  void process(const ProcessArgs &args) override;

  double ticksPerBPM (uint32_t);
  void toggle (uint8_t, bool);
  json_t *dataToJson() override;
  void dataFromJson(json_t *) override;
  void doReset();

  SynthDevKit::CV *reset;
  SynthDevKit::CV *runCV;
  SynthDevKit::CV *runParam;

  bool running = true;

  float bpm = 0;
  float width = 0;
  uint8_t count = 0;
  uint32_t tickCount = 0;
  /*
    16A: 50%
    16B: 54%
    16C: 58%
    16D: 62%
    16E: 66%
    16F: 71%
  */
  float swingTimings[6] = { 0.0f, 0.08f, 0.16f, 0.24f, 0.32f, 0.42f };
};
