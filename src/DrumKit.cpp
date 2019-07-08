#include <cstdio>

#include "DrumKit.hpp"

// The plugin-wide instance of the Plugin class
Plugin *pluginInstance;

// sample manager instance
DrumKit::SampleManager *sampleManager = nullptr;

float *loadSample (std::string which, size_t *size) {
  FILE *in = fopen(asset::plugin(pluginInstance, which).c_str(), "r");

  if (in == nullptr) {
    *size = 0;
    return nullptr;
  }

  fseek(in, 0, SEEK_END);
  *size = (size_t) ftell(in) / sizeof(float);
  rewind(in);

  if (*size == 0) {
    fclose(in);
    return nullptr;
  }

  float *arr = new float[*size]{0};

  int ret = fread(arr, sizeof(float), *size, in);

  if (*size == 0 || (size_t) ret != *size) {
    fprintf(stderr, "ERROR: cannot load file %s\n", which.c_str());
    delete[] arr;
    arr = nullptr;
    *size = 0;
  }

  fclose(in);

  return arr;
}

void setupSamples ( ) {
  DrumKit::SampleManager *sm = {0};
  sampleManager = sm->getInstance();

  // kick is 55hz
  // Sample (uint64_t _length, string _name, float *_values, float _pitch) : length(_length), name(_name), values(_values), pitch(_pitch) { }
  size_t size;
  float *arr = loadSample("res/samples/kick/01.raw", &size);
  DrumKit::Sample *kick = new DrumKit::Sample((uint64_t) size, "kick01", arr, 55);
  sampleManager->addSample(kick);

  // bd9
  for (uint8_t i = 1; i < 17; i++) {
    char buf[64];
    sprintf(buf, "%02d", i);
    arr = loadSample(std::string("res/samples/bd9/") + buf + ".raw", &size);
    DrumKit::Sample *bd9 = new DrumKit::Sample((uint64_t) size, std::string("bd9-") + buf, arr, 0);
    sampleManager->addSample(bd9);
  }

  // snare
  for (uint8_t i = 1; i < 17; i++) {
    char buf[64];
    sprintf(buf, "%02d", i);
    arr = loadSample(std::string("res/samples/snare/") + buf + ".raw", &size);
    DrumKit::Sample *snare = new DrumKit::Sample((uint64_t) size, std::string("snare-") + buf, arr, 0);
    sampleManager->addSample(snare);
  }

  // closed hh
  for (uint8_t i = 1; i < 16; i++) {
    char buf[64];
    sprintf(buf, "%02d", i);
    arr = loadSample(std::string("res/samples/closedhh/") + buf + ".raw", &size);
    DrumKit::Sample *hh = new DrumKit::Sample((uint64_t) size, std::string("closedhh-") + buf, arr, 0);
    sampleManager->addSample(hh);
  }

  // open hh
  for (uint8_t i = 1; i < 15; i++) {
    char buf[64];
    sprintf(buf, "%02d", i);
    arr = loadSample(std::string("res/samples/openhh/") + buf + ".raw", &size);
    DrumKit::Sample *hh = new DrumKit::Sample((uint64_t) size, std::string("openhh-") + buf, arr, 0);
    sampleManager->addSample(hh);
  }

  // cr78
  for (uint8_t i = 1; i < 8; i++) {
    char buf[64];
    sprintf(buf, "%02d", i);
    arr = loadSample(std::string("res/samples/cr78/") + buf + ".raw", &size);
    DrumKit::Sample *hh = new DrumKit::Sample((uint64_t) size, std::string("cr78-") + buf, arr, 0);
    sampleManager->addSample(hh);
  }

  // dmx
  for (uint8_t i = 1; i < 13; i++) {
    char buf[64];
    sprintf(buf, "%02d", i);
    arr = loadSample(std::string("res/samples/dmx/") + buf + ".raw", &size);
    DrumKit::Sample *hh = new DrumKit::Sample((uint64_t) size, std::string("dmx-") + buf, arr, 0);
    sampleManager->addSample(hh);
  }

  // tomi
  for (uint8_t i = 1; i < 15; i++) {
    char buf[64];
    sprintf(buf, "%02d", i);
    arr = loadSample(std::string("res/samples/tomi/") + buf + ".raw", &size);
    DrumKit::Sample *hh = new DrumKit::Sample((uint64_t) size, std::string("tomi-") + buf, arr, 0);
    sampleManager->addSample(hh);
  }

}


void init(rack::Plugin *p) {
  pluginInstance = p;


  setupSamples();

  // For each module, specify the ModuleWidget subclass, manufacturer slug (for
  // saving in patches), manufacturer human-readable name, module slug, and
  // module name
  p->addModel(modelBD9);
  p->addModel(modelSnare);
  p->addModel(modelClosedHH);
	p->addModel(modelOpenHH);
	p->addModel(modelDMX);
  p->addModel(modelCR78);
  p->addModel(modelSBD);
  p->addModel(modelGnome);
  p->addModel(modelSequencer);
  p->addModel(modelTomi);
  p->addModel(modelBaronial);
  //p->addModel(modelMarionette);
}
