#include "SampleManager.hpp"

#include <cmath>

namespace DrumKit {

  SampleManager* m_instanceSingleton = nullptr;

  SampleManager::SampleManager ( ) { }

  SampleManager *SampleManager::getInstance ( ) {
    return (!m_instanceSingleton) ?
      m_instanceSingleton = new SampleManager() :
      m_instanceSingleton;
  }

  void SampleManager::addSample (Sample *sample) {
    samples.push_back(sample);
  }

  Sample *SampleManager::selectSample (std::string name) {
    for (Sample *sample : samples) {
      if (sample->name == name) {
        return sample;
      }
    }

    return nullptr;
  }

  float SampleManager::step (SampleManagerContext *ctx, float tune, uint32_t sampleRate) {
    if (ctx->sample == nullptr || ctx->currentPosition < 0 || ctx->currentPosition + tune >= ctx->sample->length) {
      return 0.0f;
    }

    double adjust = ((double) ctx->sample->sampleRate / (double) sampleRate) * tune;
    double mantissa, characteristic;

    ctx->currentPosition += adjust;

    mantissa = modf(ctx->currentPosition, &characteristic);

    if ((uint64_t) characteristic == 0) {
      return 0.0f;
    }

    if (mantissa == 0) {
      return ctx->sample->values[(uint64_t) characteristic - 1];
    }

    float f1 = ctx->sample->values[(uint64_t) characteristic - 1];
    float f2 = ctx->sample->values[(uint64_t) characteristic];

    return f1 + ((f2 - f1) * mantissa);
  }

};
