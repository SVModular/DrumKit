#include "SampleController.hpp"

struct TomiModule : SampleController {
  TomiModule();
  void setupSamples( ) override;
  uint8_t sampleId(uint8_t) override;
};
