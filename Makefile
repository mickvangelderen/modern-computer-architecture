#
# Simple 2-step compilation Makefile by Mick van Gelderen
#
# Note that make does not support file paths with spaces.
#
# make - builds the target
# make TARGET_NAME - equivalent to `make`
# make clean - deletes object files and executable
# make run - runs the executable
# make sb2 - creates the executable with sb2 flags and environment
# make debug - prints some processed config variables
# make debug-VARIABLE_NAME - prints $(VARIABLE_NAME)
#

# ------------
# Config start
# ------------
EXECUTABLE_NAMES := adder x264_test
SOURCE_FOLDER := src/					# Single folder in which source files are placed, is not searched recursively, may not contain spaces.
INCLUDE_FOLDER := $(SOURCE_FOLDER)		# Single folder that gets added to the compiler path via -I, may not contain spaces.
BUILD_FOLDER := build/					# Single folder in which intermediate compilation results will be put, may not contain spaces.
TARGET_FOLDER := bin/					# Single folder in which the output file of the compilation will be put, may not contain spaces.
COMPILER := gcc							# The compiler.
COMPILER_FLAGS := -Wall -std=gnu99		# Compiler flags for the object compilation step.
COMPILER_FLAGS_SB2 := -Wall -std=gnu99 -DWORDS_BIGENDIAN		# Compiler flags for the object compilation step.
LINKER_FLAGS := 						# Compiler flags for the link step.
INCLUDES := -Isrc -Ibytecodes -Irvex -Ix264
# ---------------------
# End of config section
# ---------------------

# Strip whitespace
SOURCE_FOLDER := $(strip $(SOURCE_FOLDER))
INCLUDE_FOLDER := $(strip $(INCLUDE_FOLDER))
BUILD_FOLDER := $(strip $(BUILD_FOLDER))
TARGET_FOLDER := $(strip $(TARGET_FOLDER))
COMPILER := $(strip $(COMPILER))
COMPILER_FLAGS := $(strip $(COMPILER_FLAGS))
COMPILER_FLAGS_SB2 := $(strip $(COMPILER_FLAGS_SB2))
LINKER_FLAGS := $(strip $(LINKER_FLAGS))

# Strip trailing slash
SOURCE_FOLDER := $(SOURCE_FOLDER:%/=%)
INCLUDE_FOLDER := $(INCLUDE_FOLDER:%/=%)
BUILD_FOLDER := $(BUILD_FOLDER:%/=%)
TARGET_FOLDER := $(TARGET_FOLDER:%/=%)

# Sanity checks
ifeq ($(SOURCE_FOLDER),)
$(warning $$(SOURCE_FOLDER) defaulted to ".")
SOURCE_FOLDER := .
endif
ifeq ($(INCLUDE_FOLDER),)
$(warning $$(INCLUDE_FOLDER) defaulted to ".")
INCLUDE_FOLDER := .
endif
ifeq ($(BUILD_FOLDER),)
$(warning $$(BUILD_FOLDER) defaulted to ".")
BUILD_FOLDER := .
endif
ifeq ($(TARGET_FOLDER),)
$(warning $$(TARGET_FOLDER) defaulted to ".")
TARGET_FOLDER := .
endif
ifeq ($(COMPILER),)
$(error Cannot continue: $$(COMPILER) is empty. You need to specify a compiler when compiling!)
endif

# Find all files in the source folder ending in .c
SOURCE_FILES := $(wildcard $(SOURCE_FOLDER)/*.c)
# Find all files in the includes folder ending in .h
HEADER_FILES := $(wildcard $(INCLUDE_FOLDER)/*.h)
# Replace the source folder prefix in the source files with the build folder and replace the .c suffix by .o
OBJECT_FILES := $(SOURCE_FILES:$(SOURCE_FOLDER)/%.c=$(BUILD_FOLDER)/%.o)
# Relative path to the actual target files
EXECUTABLES := $(EXECUTABLE_NAMES:%=$(TARGET_FOLDER)/%)
# Get the working directory
PROJECT_FOLDER := $(shell pwd)

all: $(EXECUTABLES)

$(TARGET_FOLDER)/main: $(SOURCE_FOLDER)/main.c $(SOURCE_FOLDER)
	$(COMPILER) $(COMPILER_FLAGS) $(INCLUDES) -o $@ $<

$(TARGET_FOLDER)/%: $(SOURCE_FOLDER)/%.c $(HEADER_FILES) bytecodes
	sb2 $(COMPILER) $(LINKER_FLAGS) $(COMPILER_FLAGS) $(INCLUDES) \
		-o $@ \
		$< rvex/rvex.c

# Create directories if they are needed
$(TARGET_FOLDER) $(BUILD_FOLDER):
	mkdir $@

.PHONY: clean
clean:
	rm -rf $(OBJECT_FILES) $(EXECUTABLES)

BLACK := \033[0;30m
BLACKBOLD := \033[1;30m
RED := \033[0;31m
REDBOLD := \033[1;31m
GREEN := \033[0;32m
GREENBOLD := \033[1;32m
YELLOW := \033[0;33m
YELLOWBOLD := \033[1;33m
BLUE := \033[0;34m
BLUEBOLD := \033[1;34m
PURPLE := \033[0;35m
PURPLEBOLD := \033[1;35m
CYAN := \033[0;36m
CYANBOLD := \033[1;36m
WHITE := \033[0;37m
WHITEBOLD := \033[1;37m
ENDCOLOR := \033[m

debug_print_var = $(BLUEBOLD)$$($(ENDCOLOR)$1$(BLUEBOLD)) =$(ENDCOLOR) $($1)

.PHONY: debug
debug:
	@echo -e '$(YELLOWBOLD)Folders$(BLUEBOLD):$(ENDCOLOR)'
	@echo -e '	$(call debug_print_var,SOURCE_FOLDER)'
	@echo -e '	$(call debug_print_var,INCLUDE_FOLDER)'
	@echo -e '	$(call debug_print_var,BUILD_FOLDER)'
	@echo -e '	$(call debug_print_var,TARGET_FOLDER)'
	@echo -e '	$(call debug_print_var,PROJECT_FOLDER)'
	@echo -e '	$(call debug_print_var,TARGET_FOLDER)'
	@echo -e '$(YELLOWBOLD)Files$(BLUEBOLD):$(ENDCOLOR)'
	@echo -e '	$(call debug_print_var,SOURCE_FILES)'
	@echo -e '	$(call debug_print_var,HEADER_FILES)'
	@echo -e '	$(call debug_print_var,OBJECT_FILES)'
	@echo -e '	$(call debug_print_var,EXECUTABLES)'

.PHONY: debug-%
debug-%:
	@echo -e '$(call debug_print_var,$*)'
