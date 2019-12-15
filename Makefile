# Compiler and flags
COMPILER = gcc
COMPILER_FLAGS = -Wall -std=c99 -g
COMPILER_LIBRARY_FLAGS = -ldl
COMPILER_ILLEGAL_FLAGS = -Wno-unknown-pragmas
COMPILER_DYNAMIC_LIB_FLAGS = -shared -fPIC
COMPILER_DYNAMIC_LIBS_PATH = -Wl,-rpath=./$(FOLDER_BUILD)/$(FOLDER_MODULES)

# Project folders
FOLDER_SOURCES = sources
FOLDER_OBJECTS = objects
FOLDER_BUILD = build
FOLDER_HELPERS = helpers
FOLDER_LIBRARY = library
FOLDER_MODULES = modules
FOLDER_TEST = test

# Project files
SOURCES_HELPERS = $(wildcard $(FOLDER_SOURCES)/$(FOLDER_HELPERS)/*.c)
SOURCES_LIBRARY = $(wildcard $(FOLDER_SOURCES)/$(FOLDER_LIBRARY)/*.c)
SOURCES_MODULES = $(wildcard $(FOLDER_SOURCES)/$(FOLDER_MODULES)/*.c)
SOURCES_TEST = $(wildcard $(FOLDER_SOURCES)/$(FOLDER_TEST)/*.c)
OBJECTS_HELPERS = $(patsubst $(FOLDER_SOURCES)/%.c, $(FOLDER_OBJECTS)/%.o, $(SOURCES_HELPERS))
OBJECTS_LIBRARY = $(patsubst $(FOLDER_SOURCES)/%.c, $(FOLDER_OBJECTS)/%.o, $(SOURCES_LIBRARY))
MODULES = $(patsubst $(FOLDER_SOURCES)/%.c, $(FOLDER_BUILD)/%.so, $(SOURCES_MODULES))
HELPERS_NAME = helpers.lib
LIBRARY_NAME = percistency_hub.lib
TEST_NAME = malware

# Special targets
.PHONY: clean
.ONESHELL: move_test

# Default target
all: clean build_helper build_library build_modules build_test move_test

# Helpers: Alias for building the static library
build_helper: $(FOLDER_BUILD)/$(FOLDER_HELPERS)/$(HELPERS_NAME)

# Helpers: Building the static library
$(FOLDER_BUILD)/$(FOLDER_HELPERS)/$(HELPERS_NAME): $(OBJECTS_HELPERS)
	ar rcs $@ $(OBJECTS_HELPERS)

# Helpers: Compiling the sources
$(FOLDER_OBJECTS)/$(FOLDER_HELPERS)/%.o: $(FOLDER_SOURCES)/$(FOLDER_HELPERS)/%.c
	$(COMPILER) $(COMPILER_FLAGS) $(COMPILER_LIBRARY_FLAGS) $(COMPILER_ILLEGAL_FLAGS) -c $< -o $@

# Library: Alias for building the static library
build_library: $(FOLDER_BUILD)/$(FOLDER_LIBRARY)/$(LIBRARY_NAME)

# Library: Building the static library
$(FOLDER_BUILD)/$(FOLDER_LIBRARY)/$(LIBRARY_NAME): $(OBJECTS_LIBRARY)
	ar rcs $@ $(OBJECTS_LIBRARY) $(FOLDER_BUILD)/$(FOLDER_HELPERS)/$(HELPERS_NAME)

# Library: Compiling the sources
$(FOLDER_OBJECTS)/$(FOLDER_LIBRARY)/%.o: $(FOLDER_SOURCES)/$(FOLDER_LIBRARY)/%.c
	$(COMPILER) $(COMPILER_FLAGS) $(COMPILER_LIBRARY_FLAGS) $(COMPILER_ILLEGAL_FLAGS) -c $< -o $@

# Modules: Alias for building the dynamic libraries
build_modules: $(MODULES)

# Modules: Building the dynamic libraries
$(FOLDER_BUILD)/$(FOLDER_MODULES)/%.so: $(FOLDER_SOURCES)/$(FOLDER_MODULES)/%.c
	$(COMPILER) $(COMPILER_FLAGS) $(COMPILER_DYNAMIC_LIB_FLAGS) $(COMPILER_ILLEGAL_FLAGS) $< $(FOLDER_BUILD)/$(FOLDER_HELPERS)/$(HELPERS_NAME) -o $@

# Test: Building the test executable
build_test:
	$(COMPILER) $(COMPILER_FLAGS) $(COMPILER_LIBRARY_FLAGS) $(COMPILER_ILLEGAL_FLAGS) $(COMPILER_DYNAMIC_LIBS_PATH) $(SOURCES_TEST) $(FOLDER_BUILD)/$(FOLDER_LIBRARY)/$(LIBRARY_NAME) $(FOLDER_BUILD)/$(FOLDER_HELPERS)/$(HELPERS_NAME) -o $(FOLDER_BUILD)/$(FOLDER_TEST)/$(TEST_NAME)

# Test: move the executable to project root
move_test: build_test
	mv $(FOLDER_BUILD)/$(FOLDER_TEST)/$(TEST_NAME) .

# Target for cleaning the project
clean:
	rm -f $(FOLDER_BUILD)/$(FOLDER_HELPERS)/*
	rm -f $(FOLDER_BUILD)/$(FOLDER_LIBRARY)/*
	rm -f $(FOLDER_BUILD)/$(FOLDER_MODULES)/*
	rm -f $(FOLDER_BUILD)/$(FOLDER_TEST)/*
	rm -f $(FOLDER_OBJECTS)/$(FOLDER_HELPERS)/*
	rm -f $(FOLDER_OBJECTS)/$(FOLDER_LIBRARY)/*
	rm -f $(FOLDER_OBJECTS)/$(FOLDER_MODULES)/*
	rm -f $(FOLDER_OBJECTS)/$(FOLDER_TEST)/*
	rm -f $(TEST_NAME)*
	rm -f .gdb_history
	rm -f peda-session-$(TEST_NAME).txt