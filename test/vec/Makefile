# === Config ===
CCOLL := vec 
# === Config ===
TOP_DIR := ../..
LIB_NAME := lib$(CCOLL).a
LIB_PATH := $(TOP_DIR)/build/lib/$(LIB_NAME)
BIN := exe
# === Tools ===
CC := gcc
CFLAGS := -Wall -Wextra -O2
LDFLAGS := -L$(TOP_DIR)/build/lib -l$(CCOLL)
# === Modules ===
SRC := test.c
# === Color definitions ===
COLOR_CC    := \e[38;5;5m
COLOR_ARROW := \e[38;5;5m
COLOR_OK    := \e[38;5;2m
COLOR_MAIN  := \e[38;5;3m
COLOR_RESET := \e[0m
# === Targets ===
all: $(BIN)

# Build top-level component if static lib is missing
$(LIB_PATH):
	@echo -e "[$(COLOR_MAIN)INFO$(COLOR_RESET)] Building '$(CCOLL)' via top-level Makefile"
	@$(MAKE) -C $(TOP_DIR) $(CCOLL)

# Build the local test executable
$(BIN): $(SRC) $(LIB_PATH)
	@$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)
	@echo -e "$(COLOR_CC)$(CC)$(COLOR_RESET) $< $(COLOR_ARROW)->$(COLOR_RESET) $@"
	@./exe

deep:
	@echo -e "[$(COLOR_MAIN)INFO$(COLOR_RESET)] Forcing rebuild of '$(CCOLL)' via top-level Makefile"
	@$(MAKE) -C $(TOP_DIR) clean
	@$(MAKE) -C $(TOP_DIR) $(CCOLL)
	@echo -e "[$(COLOR_OK)OK$(COLOR_RESET)] Rebuilt top-level '$(CCOLL)'"
	@$(MAKE) $(BIN)

run:
	@echo -e "$(COLOR_CC)run$(COLOR_RESET):$(COLOR_MAIN)$(BIN)$(COLOR_RESET)"
	@./exe

clean:
	@rm -f $(BIN)
	@echo -e "[$(COLOR_OK)OK$(COLOR_RESET)] Cleaned all build artifacts"

.PHONY: all clean deep
