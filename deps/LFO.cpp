#include "LFO.hpp"

#include <cstdlib>
#include <ctime>

#define clamp(val, min, max) (val < min ? min : (val > max ? max : val))

LowFrequencyOscillator::LowFrequencyOscillator() {
  phase = 0.0;
  pw = 0.5;
  freq = 1.0;
  offset = false;
  invert = false;
  shift = 0.0;
  shiftVal = 0.0;
  random = 0.0f;
  skip = false;
  cv = new SynthDevKit::CV(1.7f);
  srand(time(NULL));
}

LowFrequencyOscillator::~LowFrequencyOscillator() {
  delete cv;
}

void LowFrequencyOscillator::setPitch(float pitch) {
  pitch = fminf(pitch, 8.0);
  freq = powf(2.0, pitch);
}

void LowFrequencyOscillator::setFrequency(float frequency) {
  freq = frequency;
}

void LowFrequencyOscillator::setPulseWidth(float pw_) {
  const float pwMin = 0.01;
  pw = clamp(pw_, pwMin, 1.0f - pwMin);
}

void LowFrequencyOscillator::setShift(float shift_) {
  shift = clamp(shift_, 0.0f, freq);
}

void LowFrequencyOscillator::setRandom(float random_) {
  random = clamp(random_, 0.0f, 10.0f);
}

void LowFrequencyOscillator::setInvert(bool invert_) {
  invert = invert_;
}

void LowFrequencyOscillator::setReset(float reset) {
  cv->update(reset);
  if (cv->newTrigger()) {
    phase = 0.0;
    shiftVal = 0.0;
  }
}

void LowFrequencyOscillator::hardReset() {
  phase = 0.0;
  shiftVal = 0.0;
}

void LowFrequencyOscillator::step(float dt) {
  if (shiftVal <= shift) {
    shiftVal++;
    return;
  }

  float deltaPhase = fminf(freq * dt, 0.5);
  phase += deltaPhase;
  if (phase >= 1.0) {
    phase -= 1.0;
    if (random && random >= (rand() % 50)) {
      skip = true;
    } else {
      skip = false;
    }
  }
}

float LowFrequencyOscillator::sin() {
  if (skip) {
    return 0.0f;
  }

  if (offset) {
    return 1.0 - cosf(2 * M_PI * phase) * (invert ? -1.0 : 1.0);
  } else {
    return sinf(2 * M_PI * phase) * (invert ? -1.0 : 1.0);
  }
}

float LowFrequencyOscillator::tri(float x) {
  return 4.0 * fabsf(x - roundf(x));
}

float LowFrequencyOscillator::tri() {
  if (skip) {
    return 0.0f;
  }

  if (offset) {
    return tri(invert ? phase - 0.5 : phase);
  } else {
    return -1.0 + tri(invert ? phase - 0.25 : phase - 0.75);
  }
}

float LowFrequencyOscillator::saw(float x) {
  return 2.0 * (x - roundf(x));
}

float LowFrequencyOscillator::saw() {
  if (skip) {
    return 0.0f;
  }

  if (offset) {
    return invert ? 2.0 * (1.0 - phase) : 2.0 * phase;
  } else {
    return saw(phase) * (invert ? -1.0 : 1.0);
  }
}

float LowFrequencyOscillator::sqr() {
  if (skip) {
    return 0.0f;
  }

  float sqr = (phase < pw) ^ invert ? 1.0 : -1.0;
  return offset ? sqr + 1.0 : sqr;
}

float LowFrequencyOscillator::progress() {
  return phase;
}
