# === Config ===
NAME        := ccoll
BUILD_DIR   := build
SRC_DIR     := src
INCLUDE_DIR := include
OBJ_DIR     := $(BUILD_DIR)/obj
LIB_DIR     := $(BUILD_DIR)/lib

# === Tools ===
CC          := gcc
AR          := ar
RANLIB      := ranlib
CFLAGS      := -Wall -Wextra -std=c11 -O2 -I$(INCLUDE_DIR)
ARFLAGS     := rcs

# === Modules ===
MODULES     := vec minivec lli

# === Color definitions ===
COLOR_CC    := \e[38;5;5m
COLOR_AR    := \e[38;5;5m
COLOR_ARROW := \e[38;5;6m
COLOR_OK    := \e[38;5;2m
COLOR_MAIN  := \e[38;5;3m
COLOR_RESET := \e[0m

# === Main targets ===
.PHONY: all clean ccoll $(MODULES) help

all: $(MODULES) ccoll

help:
	@echo "Available targets:"
	@echo "  all      - Build all modules and main library"
	@echo "  ccoll    - Build main library (combines all modules)"
	@echo "  vec      - Build vec module"
	@echo "  lli      - Build lli module"
	@echo "  clean    - Remove all build artifacts"
	@echo "  help     - Show this help message"

# === Module libraries ===
$(MODULES): %: $(LIB_DIR)/lib%.a

# Find all C files for a module and convert to object files
define MODULE_SOURCES
$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(wildcard $(SRC_DIR)/$(1)/*.c $(SRC_DIR)/$(1)/**/*.c))
endef

# Create individual module libraries
$(LIB_DIR)/lib%.a: | $(LIB_DIR)
	$(eval MODULE_OBJS := $(call MODULE_SOURCES,$*))
	@if [ -z "$(MODULE_OBJS)" ]; then \
		echo "Warning: No source files found for module $*"; \
		exit 1; \
	fi
	@$(MAKE) $(MODULE_OBJS)
	@$(AR) $(ARFLAGS) $@ $(MODULE_OBJS)
	@echo -e "$(COLOR_AR)$(AR)$(COLOR_RESET) $@"
	@for obj in $(MODULE_OBJS); do \
		echo -e "  $(COLOR_ARROW)>$(COLOR_RESET) $$obj"; \
	done
	@$(RANLIB) $@
	@echo -e "[$(COLOR_OK)OK$(COLOR_RESET)] Built $(COLOR_OK)$@$(COLOR_RESET)"

# === Main library (combines all modules) ===
# Collect all object files from all modules
ALL_OBJS := $(foreach module,$(MODULES),$(call MODULE_SOURCES,$(module)))

ccoll: $(LIB_DIR)/lib$(NAME).a

$(LIB_DIR)/lib$(NAME).a: $(ALL_OBJS) | $(LIB_DIR)
	@$(AR) $(ARFLAGS) $@ $^
	@echo -e "$(COLOR_AR)$(AR)$(COLOR_RESET) $@"
	@for obj in $^; do \
		echo -e "  $(COLOR_ARROW)>$(COLOR_RESET) $$obj"; \
	done
	@$(RANLIB) $@
	@echo -e "[$(COLOR_OK)OK$(COLOR_RESET)] Built $(COLOR_OK)$@$(COLOR_RESET) ($(COLOR_MAIN)Main$(COLOR_RESET))"

# === Object file compilation ===
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo -e "$(COLOR_CC)$(CC)$(COLOR_RESET) $< $(COLOR_CC)->$(COLOR_RESET) $@"

# === Directory creation ===
$(BUILD_DIR) $(OBJ_DIR) $(LIB_DIR):
	@mkdir -p $@

# === Dependency generation ===
DEPS := $(ALL_OBJS:.o=.d)

# Generate dependency files
$(OBJ_DIR)/%.d: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -MM -MT $(patsubst %.d,%.o,$@) $< > $@

# Include dependency files (ignore if they don't exist)
-include $(DEPS)

# === Clean ===
clean:
	@rm -rf $(BUILD_DIR)
	@echo -e "[$(COLOR_OK)OK$(COLOR_RESET)] Cleaned all build artifacts"

# === Debug info ===
.PHONY: debug
debug:
	@echo "MODULES: $(MODULES)"
	@echo "ALL_OBJS: $(ALL_OBJS)"
	@for module in $(MODULES); do \
		echo "Module $$module objects: $(call MODULE_SOURCES,$$module)"; \
	done
