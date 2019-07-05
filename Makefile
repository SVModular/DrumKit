# FLAGS will be passed to both the C and C++ compiler
FLAGS +=
CFLAGS +=
CXXFLAGS +=

# Careful about linking to libraries, since you can't assume much about the user's environment and library search path.
# Static libraries are fine.
LDFLAGS +=

# Controllers
CONTROLLERS += $(wildcard src/controller/*.cpp)

# SynthDevKit
SYNTHDEVKIT += $(wildcard deps/SynthDevKit/src/*.cpp)

# Views
VIEWS += $(wildcard src/view/*.cpp)

# Models
MODELS += $(wildcard src/model/*.cpp)

# Add .cpp and .c files to the build
SOURCES += $(wildcard src/*.cpp) $(SYNTHDEVKIT) $(CONTROLLERS) $(VIEWS) $(MODELS) $(wildcard deps/*.cpp)

# Add files to the ZIP package when running `make dist`
# The compiled plugin is automatically added.
DISTRIBUTABLES += $(wildcard LICENSE*) res

# Must include the VCV plugin Makefile framework
RACK_DIR ?= ../..
include $(RACK_DIR)/plugin.mk
