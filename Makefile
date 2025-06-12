TARGET_EXEC ?= main

BUILD_DIR ?= ./build
SRC_DIRS ?= .

# Tous les .c dans le répertoire courant
SRCS := $(wildcard *.c)
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# Include = répertoire courant
INC_FLAGS := -I.

# Ajout de -Wall -Werror -Wextra
CFLAGS ?= $(INC_FLAGS) -MMD -MP -O3 -Wall -Werror -Wextra

# L'exécutable est dans le répertoire courant
$(TARGET_EXEC): $(OBJS)
	$(MKDIR_P) $(dir $@)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Compilation des .c en .o dans build/
$(BUILD_DIR)/%.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR) $(TARGET_EXEC)

-include $(DEPS)

MKDIR_P ?= mkdir -p
