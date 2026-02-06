# Corrode — TB-303 acid synth for Daisy Patch Init
# Project Name
TARGET = corrode

# Library Locations
LIBDAISY_DIR = lib/libDaisy
DAISYSP_DIR = lib/DaisySP

# Sources
CPP_SOURCES = \
	src/main.cpp \
	src/corrode.cpp \
	src/controls.cpp \
	src/midi_handler.cpp \
	dsp/open303.cpp \
	dsp/tee_bee_filter.cpp \
	dsp/blep_oscillator.cpp \
	dsp/analog_envelope.cpp \
	dsp/decay_envelope.cpp \
	dsp/one_pole_filter.cpp \
	dsp/leaky_integrator.cpp \
	dsp/aa_filter.cpp

# Includes
C_INCLUDES = \
	-Isrc \
	-Idsp

# Compiler flags
OPT = -O2
CPPFLAGS += -std=gnu++17

# Debug build support
ifdef DEBUG
OPT = -Og -g3
CPPFLAGS += -DDEBUG
endif

# Core Makefile (from libDaisy)
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
