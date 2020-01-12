# Compiler and flags
COMPILER = gcc
COMPILER_FLAGS = -Wall -std=c99 -g -D_POSIX_SOURCE
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
FOLDER_MALWARE = malware

# Project files
SOURCES_HELPERS = $(wildcard $(FOLDER_SOURCES)/$(FOLDER_HELPERS)/*.c)
SOURCES_LIBRARY = $(wildcard $(FOLDER_SOURCES)/$(FOLDER_LIBRARY)/*.c)
SOURCES_MODULES = $(wildcard $(FOLDER_SOURCES)/$(FOLDER_MODULES)/*.c)
SOURCES_MALWARE = $(wildcard $(FOLDER_SOURCES)/$(FOLDER_MALWARE)/*.c)
OBJECTS_HELPERS = $(patsubst $(FOLDER_SOURCES)/%.c, $(FOLDER_OBJECTS)/%.o, $(SOURCES_HELPERS))
OBJECTS_LIBRARY = $(patsubst $(FOLDER_SOURCES)/%.c, $(FOLDER_OBJECTS)/%.o, $(SOURCES_LIBRARY))
MODULES = $(patsubst $(FOLDER_SOURCES)/%.c, $(FOLDER_BUILD)/%.so, $(SOURCES_MODULES))
HELPERS_NAME = helpers.lib
LIBRARY_NAME = persistency_hub.lib
MALWARE_NAME = malware

# Special targets
.PHONY: clean
.ONESHELL: move_malware

# Default target
all: clean build_helper build_library build_modules build_malware move_malware

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

# Malware: Building the malware executable
build_malware:
	$(COMPILER) $(COMPILER_FLAGS) $(COMPILER_LIBRARY_FLAGS) $(COMPILER_ILLEGAL_FLAGS) $(COMPILER_DYNAMIC_LIBS_PATH) $(SOURCES_MALWARE) $(FOLDER_BUILD)/$(FOLDER_LIBRARY)/$(LIBRARY_NAME) $(FOLDER_BUILD)/$(FOLDER_HELPERS)/$(HELPERS_NAME) -o $(FOLDER_BUILD)/$(FOLDER_MALWARE)/$(MALWARE_NAME)

# Malware: move the executable to project root
move_malware: build_malware
	mv $(FOLDER_BUILD)/$(FOLDER_MALWARE)/$(MALWARE_NAME) .

# Target for cleaning the project
clean:
	rm -f $(FOLDER_BUILD)/$(FOLDER_HELPERS)/*
	rm -f $(FOLDER_BUILD)/$(FOLDER_LIBRARY)/*
	rm -f $(FOLDER_BUILD)/$(FOLDER_MODULES)/*
	rm -f $(FOLDER_BUILD)/$(FOLDER_MALWARE)/*
	rm -f $(FOLDER_OBJECTS)/$(FOLDER_HELPERS)/*
	rm -f $(FOLDER_OBJECTS)/$(FOLDER_LIBRARY)/*
	rm -f $(FOLDER_OBJECTS)/$(FOLDER_MODULES)/*
	rm -f $(FOLDER_OBJECTS)/$(FOLDER_MALWARE)/*
	rm -f $(MALWARE_NAME)*
	rm -f .gdb_history
	rm -f peda-session-$(MALWARE_NAME).txt