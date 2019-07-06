#pragma once

#include <cstdint>
#include <cmath>

enum EnvelopeType {
  ATTACK,
  DECAY,
  SUSTAIN,
  RELEASE,
  OFF
};

enum CurveType {
  LINEAR,
  CURVED
};

class Envelope {
public:
  Envelope ( ) {
    peakLevel = 0;

    attackTime = 0;
    decayTime = 0;
    sustainTime = 0;
    releaseTime = 0;

    sustainLevel = 0;

    attackCurve = LINEAR;
    decayCurve = LINEAR;
    releaseCurve = LINEAR;

    current = OFF;
  }

  void setPeakLevel (float l) {
    peakLevel = l;
  }

  void setAttackTime (uint64_t a) {
    attackTime = a;
  }

  void setAttackCurve (CurveType t) {
    attackCurve = t;
  }

  void setDecayTime (uint64_t d) {
    decayTime = d;
  }

  void setDecayCurve (CurveType t) {
    decayCurve = t;
  }

  void setSustainTime (uint64_t s) {
    sustainTime = s;
  }

  void setSustainLevel (float s) {
    sustainLevel = s;
  }

  void setReleaseTime (uint64_t r) {
    releaseTime = r;
  }

  void setReleaseCurve (CurveType t) {
    releaseCurve = t;
  }

  void trigger ( ) {
    current = ATTACK;
    currentAttackPosition = 0;
    currentDecayPosition = 0;
    currentSustainPosition = 0;
    currentReleasePosition = 0;
    lastValue = 0;
  }

  float step ( ) {
    switch (current) {
    case ATTACK:
      lastValue = attackValue();
      break;

    case DECAY:
      lastValue = decayValue();
      break;

    case SUSTAIN:
      lastValue = sustainValue();
      break;

    case RELEASE:
      lastValue = releaseValue();
      break;

    default:
      lastValue = 0;
    }

    return lastValue;
  }

  EnvelopeType getCurrentEnvelopeType ( ) {
    return current;
  }

private:
  float peakLevel;
  uint64_t attackTime;
  uint64_t decayTime;
  uint64_t sustainTime;
  float sustainLevel;
  uint64_t releaseTime;
  float lastValue;
  float finalAttack;
  uint64_t currentAttackPosition;
  uint64_t currentDecayPosition;
  uint64_t currentSustainPosition;
  uint64_t currentReleasePosition;
  EnvelopeType current;
  CurveType attackCurve;
  CurveType decayCurve;
  CurveType releaseCurve;


  float attackValue ( ) {
    // if we are in some unknown state, just return the last value
    // this can happen if values change between steps
    if (currentAttackPosition >= attackTime) {
      return lastValue;
    }

    float returnValue;

    if (attackCurve == CURVED) {
      float percentage = (float) (currentAttackPosition + 1) / (float) attackTime;

      returnValue = sinf(percentage * 0.5 * M_PI);
    } else if (attackCurve == LINEAR) {
      // calculate linear value
      returnValue = (float) (currentAttackPosition + 1) / (float) attackTime;
    } else {
      returnValue = 1;
    }

    currentAttackPosition++;
    if (currentAttackPosition >= attackTime) {
      finalAttack = returnValue;
      current = DECAY;
    }

    return returnValue;
  }

  float decayValue ( ) {
    // if we are in some unknown state, just return last value
    if (currentDecayPosition >= decayTime) {
      return lastValue;
    }

    float returnValue;

    if (finalAttack < sustainLevel) {
      returnValue = finalAttack;
    } else {
      if (decayCurve == CURVED) {
        // find the percentage between the last value and the sustain level
        float percentage = (float) (currentDecayPosition + 1) / (float) decayTime;

        returnValue = fabs(1 - (sinf(percentage * 0.5 * M_PI) * (1 - sustainLevel)));
      } else if (decayCurve == LINEAR) {
        float percentage = (float) (decayTime - currentDecayPosition - 1) / (float) decayTime;

        returnValue = (percentage * (finalAttack - sustainLevel)) + sustainLevel;
      } else {
        returnValue = sustainLevel;
      }
    }

    currentDecayPosition++;
    if (currentDecayPosition >= decayTime) {
      current = SUSTAIN;
    }

    return returnValue;
  }

  float sustainValue ( ) {
    // if we are in some unknown state, just return last value
    if (currentSustainPosition >= sustainTime) {
      return lastValue;
    }

    currentSustainPosition++;
    if (currentSustainPosition >= sustainTime) {
      current = RELEASE;
    }

    return sustainLevel;
  }

  float releaseValue ( ) {
    // if we are in some unknown state, just return 0, we're at the end
    if (currentReleasePosition >= releaseTime) {
      return 0;
    }

    float returnValue;

    if (releaseCurve == CURVED) {
      // find the percentage between the last value and the sustain level
      float percentage = (float) (currentReleasePosition + 1) / (float) releaseTime;

      returnValue = fabs(1 - (sinf(percentage * 0.5 * M_PI) * (sustainLevel))) - (1 - sustainLevel);
    } else if (releaseCurve == LINEAR) {
      float percentage = (float) (releaseTime - currentReleasePosition - 1) / (float) releaseTime;

      returnValue = (percentage * sustainLevel);
    } else {
      returnValue = 0;
    }

    currentReleasePosition++;
    if (currentReleasePosition >= releaseTime) {
      current = OFF;
    }

    return returnValue;
  }

};
