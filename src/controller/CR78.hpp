#include "SampleController.hpp"

struct CR78Module : SampleController {
  CR78Module();
  void setupSamples( ) override;
  uint8_t sampleId(uint8_t) override;
};
