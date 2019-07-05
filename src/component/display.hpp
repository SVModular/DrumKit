#pragma once

struct BPMDisplay : TransparentWidget {
  float *value;
  std::shared_ptr<Font> font;

  BPMDisplay ( ) {
    value = NULL;
    font = APP->window->loadFont(asset::plugin(pluginInstance, "res/component/NovaMono.ttf"));
  }

  void draw (const DrawArgs &args) override {
    char text[12];
    nvgFontSize(args.vg, 38);
    nvgFontFaceId(args.vg, font->handle);
    nvgTextLetterSpacing(args.vg, 1);

    nvgBeginPath(args.vg);
    nvgFillColor(args.vg, nvgRGBA(0x00, 0xff, 0x00, 0xff));

    if (value) {
      sprintf(text, "%03d", (uint8_t) *value);
    } else {
      sprintf(text, "115");
    }

    nvgText(args.vg, 0, 0, text, NULL);
  }
};

struct WidthDisplay : TransparentWidget {
  float *value;

  WidthDisplay ( ) {
    value = NULL;
  }

  void draw (const DrawArgs &args) override {
    // 68 width - default to half way for no value
    uint8_t width = 34;
    if (value) {
      width = (uint8_t) (68 * *value);
    }

    nvgStrokeColor(args.vg, nvgRGBA(0x00, 0xff, 0x00, 0xff));
    nvgStrokeWidth(args.vg, 1.0f);
    nvgBeginPath(args.vg);
    nvgMoveTo(args.vg, 1, 20);
    nvgLineTo(args.vg, 1, 5);
    nvgLineTo(args.vg, 1 + width, 5);
    nvgLineTo(args.vg, 1 + width, 20);
    nvgLineTo(args.vg, 69, 20);
    nvgStroke(args.vg);
  }
};

struct TrackDisplay : TransparentWidget {
  int8_t *value;
  std::shared_ptr<Font> font;

  TrackDisplay ( ) {
    value = NULL;
    font = APP->window->loadFont(asset::plugin(pluginInstance, "res/component/NovaMono.ttf"));
  }

  void draw (const DrawArgs &args) override {
    char text[12];
    nvgFontSize(args.vg, 38);
    nvgFontFaceId(args.vg, font->handle);
    nvgTextLetterSpacing(args.vg, 1);

    nvgBeginPath(args.vg);
    nvgFillColor(args.vg, nvgRGBA(0x00, 0xff, 0x00, 0xff));

    if (value) {
      if (*value) {
        sprintf(text, "%02d", *value);
      } else {
        sprintf(text, "--");
      }
    } else {
      sprintf(text, "01");
    }

    nvgText(args.vg, 0, 0, text, NULL);
  }
};

struct SmallTrackDisplay : TransparentWidget {
  int8_t *value;
  std::shared_ptr<Font> font;

  SmallTrackDisplay ( ) {
    value = NULL;
    font = APP->window->loadFont(asset::plugin(pluginInstance, "res/component/NovaMono.ttf"));
  }

  void draw (const DrawArgs &args) override {
    char text[12];
    nvgFontSize(args.vg, 19);
    nvgFontFaceId(args.vg, font->handle);
    nvgTextLetterSpacing(args.vg, 1);

    nvgBeginPath(args.vg);
    nvgFillColor(args.vg, nvgRGBA(0x00, 0xff, 0x00, 0xff));

    if (value) {
      if (*value) {
        sprintf(text, "%02d", *value);
      } else {
        sprintf(text, "--");
      }
    } else {
      sprintf(text, "01");
    }

    nvgText(args.vg, 0, 0, text, NULL);
  }
};
