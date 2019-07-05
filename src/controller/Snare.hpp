#include "SampleController.hpp"

struct SnareModule : SampleController {
  SnareModule();
  void setupSamples( ) override;
  uint8_t sampleId(uint8_t) override;
};
