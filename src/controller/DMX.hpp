#include "SampleController.hpp"

struct DMXModule : SampleController {
  DMXModule();
  void setupSamples( ) override;
  uint8_t sampleId(uint8_t) override;
};
