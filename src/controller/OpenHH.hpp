#include "SampleController.hpp"

struct OpenHHModule : SampleController {
  OpenHHModule();
  void setupSamples( ) override;
  uint8_t sampleId(uint8_t) override;
};
