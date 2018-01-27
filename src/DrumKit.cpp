#include "DrumKit.hpp"

// The plugin-wide instance of the Plugin class
Plugin *plugin;

void init(rack::Plugin *p) {
  plugin = p;
  // This is the unique identifier for your plugin
  p->slug = "DrumKit";
#ifdef VERSION
  p->version = TOSTRING(VERSION);
#endif
  p->website = "https://github.com/JerrySievert/DrumKit";
  p->manual  = "https://github.com/JerrySievert/DrumKit/blob/master/README.md";

  // For each module, specify the ModuleWidget subclass, manufacturer slug (for
  // saving in patches), manufacturer human-readable name, module slug, and
  // module name
  p->addModel(createModel<BD9Widget>("DrumKit", "Bass Drum 9", "Bass Drum 9"));
  p->addModel(
      createModel<SnareWidget>("DrumKit", "Snare Drum N", "Snare Drum N"));
  p->addModel(
      createModel<ClosedHHWidget>("DrumKit", "Closed HiHat", "Closed HiHat"));
	p->addModel(
      createModel<OpenHHWidget>("DrumKit", "Open HiHat", "Open HiHat"));

	p->addModel(
      createModel<DMXWidget>("DrumKit", "DMX", "DMX"));

  // Any other plugin initialization may go here.
  // As an alternative, consider lazy-loading assets and lookup tables when your
  // module is created to reduce startup times of Rack.
}
