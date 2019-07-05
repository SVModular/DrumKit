#include "SampleController.hpp"

struct BD9Module : SampleController {
  BD9Module();
  void setupSamples( ) override;
  uint8_t sampleId(uint8_t) override;
};
