#include "DrumKit.hpp"

// The plugin-wide instance of the Plugin class
Plugin *plugin;

void init(rack::Plugin *p) {
  pluginInstance = p;
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
  p->addModel(modelDMXmk2);
  p->addModel(modelCR78mk2);

  // Any other plugin initialization may go here.
  // As an alternative, consider lazy-loading assets and lookup tables when your
  // module is created to reduce startup times of Rack.
}
