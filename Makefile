CC := gcc
CFLAGS := -Wall -Werror
LDFLAGS := -lncurses
DEBUG_CFLAGS := -O0 -g -ggdb
RELEASE_CFLAG := -O2 -DNDEBUG
BUILD_DIR ?= .build
SRC_DIR := src
SRCS := sl.c
HDRS := animation.h train.h
TARGET := sl

RELEASE ?= 0
ifeq ($(RELEASE), 1)
    CFLAGS += $(RELEASE_CFLAGS)
    TARGET_BUILD_DIR := $(BUILD_DIR)/release
else
    CFLAGS += $(DEBUG_CFLAGS)
    TARGET_BUILD_DIR += $(BUILD_DIR)/debug
endif

OBJS := $(SRCS:%.c=$(TARGET_BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

all: $(TARGET_BUILD_DIR)/$(TARGET)

$(TARGET_BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(TARGET_BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGET_BUILD_DIR)

distclean:
	rm -rf $(BUILD_DIR)

.PHONY: clean

MKDIR_P ?= mkdir -p
