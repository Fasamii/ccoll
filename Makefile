NAME = ccoll
BUILD_DIR = build
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = $(BUILD_DIR)/obj
LIB_DIR = $(BUILD_DIR)/lib

CC = gcc
AR = ar
CFLAGS = -Wall -Wextra -std=c99 -O2 -I$(INCLUDE_DIR)
ARFLAGS = rcs

MODULES := vec llist

# === Build Targets ===
.PHONY: all clean $(MODULES) ccoll

# Default: build all modules
all: ccoll

# Build full ccoll library (all modules)
ccoll: $(addprefix $(LIB_DIR)/lib,$(addsuffix .a,$(MODULES)))
	@mkdir -p $(LIB_DIR)
	$(AR) $(ARFLAGS) $(LIB_DIR)/lib$(NAME).a $^
	@echo "[✓] Built static lib: $(LIB_DIR)/lib$(NAME).a"

# Build individual module archives
$(MODULES):
	$(MAKE) module MODULE=$@

module:
	@echo "[+] Building module: $(MODULE)"
	$(MAKE) $(LIB_DIR)/lib$(MODULE).a

# Rule to build a single module's static lib
$(LIB_DIR)/lib%.a: $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(shell find $(SRC_DIR)/$* -name '*.c'))
	@mkdir -p $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $^

# Rule to build each object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR)
	@echo "[✓] Cleaned build artifacts."
