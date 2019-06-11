#include "VDrumModule.hpp"

struct CR78mk2Module : VDrumModule {
  float notes[12] = { 0.08, 0.17, 0.25, 0.33, 0.42, 0.5, 0.58, 0.67, 0.75, 0.83, 0.92, 1.0 };

  CR78mk2Module();

  void setupSamples( ) override;

  struct VDrumContainer *getSample(float current) override;
};
