# Compiler options
CC = gcc
CFLAGS = -Wall -std=c99 -g
ILLEGAL_FLAGS = -Wno-unknown-pragmas

# Project folders
SOURCE_FOLDER = sources
OBJECTS_FOLDER = objects
BUILD_FOLDER = build
TESTS_FOLDER = tests

# Project files
SOURCES := $(wildcard $(SOURCE_FOLDER)/*.c)
OBJECTS := $(patsubst $(SOURCE_FOLDER)/%.c, $(OBJECTS_FOLDER)/%.o, $(SOURCES))
LIBRARY = percistency_hub.lib

# Malware project files
MALWARE_SOURCE = malware.c
MALWARE = malware

# Special targets
.PHONY: clean
.ONESHELL: build_malware

# Default target
all: clean build_library build_malware

# Target for building
build_library: $(BUILD_FOLDER)/$(LIBRARY)

build_malware:
	cd $(TESTS_FOLDER)
	$(CC) $(CFLAGS) $(ILLEGAL_FLAGS) $(MALWARE_SOURCE) ../$(BUILD_FOLDER)/$(LIBRARY) -o $(MALWARE)
	mv $(MALWARE) ../
	cd ../

# Target for building the static linked library
$(BUILD_FOLDER)/$(LIBRARY): $(OBJECTS)
	ar rcs $@ $(OBJECTS)

# Target for compiling the sources
$(OBJECTS_FOLDER)/%.o: $(SOURCE_FOLDER)/%.c
	$(CC) $(CFLAGS) $(ILLEGAL_FLAGS) -c $< -o $@

# Target for cleaning the project
clean:
	rm -f $(BUILD_FOLDER)/*
	rm -f $(OBJECTS_FOLDER)/*
	rm -f $(MALWARE)
	rm -f .gdb_history
	rm -f peda-session-$(MALWARE).txt