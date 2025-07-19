# === Config ===
NAME       := ccoll
BUILD_DIR  := build
SRC_DIR    := src
INCLUDE_DIR:= include
OBJ_DIR    := $(BUILD_DIR)/obj
LIB_DIR    := $(BUILD_DIR)/lib

# === Tools ===
CC     := gcc
AR     := ar
RANLIB := ranlib
CFLAGS := -Wall -Wextra -std=c99 -O2 -I$(INCLUDE_DIR)
ARFLAGS:= rcs

MODULES := vec 

.PHONY: all clean ccoll $(MODULES)

# Default: build everything (individual libs + combined lib)
all: $(MODULES) ccoll

# Build individual module libraries
$(MODULES): %: $(LIB_DIR)/lib%.a

# Pattern rule for individual module archives
$(LIB_DIR)/lib%.a: $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(shell find $(SRC_DIR)/$* -name '*.c'))
	@mkdir -p $(LIB_DIR)
	@$(AR) $(ARFLAGS) $@ $^
	@echo -e "\e[38;5;5m"$(AR)"\e[0m  "$(LIB_DIR)" \e[38;5;5->\e[0m "$^
	@$(RANLIB) $@
	@echo -e "[\e[38;5;2mOK\e[0m] Built \e[38;5;2m"$@"\e[0m"

# Collect all object files from all modules
ALL_OBJS := $(foreach module,$(MODULES),$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(shell find $(SRC_DIR)/$(module) -name '*.c')))

# Combined super-library (builds individual libs as dependencies but only outputs ccoll.a)
ccoll: $(ALL_OBJS)
	@mkdir -p $(LIB_DIR)
	@$(AR) $(ARFLAGS) $(LIB_DIR)/lib$(NAME).a $^
	@echo -e "\e[38;5;5m"$(AR)"\e[0m "$(LIB_DIR)" \e[38;5;5m->\e[0m "$^
	@$(RANLIB) $(LIB_DIR)/lib$(NAME).a
	@echo -e "[\e[38;5;2mOK\e[0m] Built \e[38;5;2m"$(LIB_DIR)/lib$(NAME).a"\e[0m (\e[38;5;3mMain\e[0m)"

# Compile .c → .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo -e "\e[38;5;5m"$(CC)"\e[0m "$<" \e[38;5;5m->\e[0m "$@

clean:
	@rm -rf $(BUILD_DIR)
	@echo -e "[\e[38;5;2mOK\e[0m] Cleaned all build artifacts"
