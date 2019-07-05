#include "SampleController.hpp"

struct ClosedHHModule : SampleController {
  ClosedHHModule();
  void setupSamples( ) override;
  uint8_t sampleId(uint8_t) override;
};
