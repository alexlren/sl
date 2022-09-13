BUILD_DIR ?= .build
SRC_DIR := src
ANIMS_DIR := animations
ANIMS_BUILD_DIR := $(SRC_DIR)/generated
ANIM_ROOT_FILE := anims.h
TOOLS_DIR := tools

CC := gcc
CFLAGS := -Wall -Werror -I$(SRC_DIR)
LDFLAGS := -lncurses
DEBUG_CFLAGS := -O0 -g -ggdb
RELEASE_CFLAG := -O2 -DNDEBUG
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

ANIMS := $(dir $(wildcard $(ANIMS_DIR)/*/0.slf))
ANIMS := $(patsubst %/,%,$(ANIMS))

ANIM_HDRS := $(ANIMS:$(ANIMS_DIR)/%=$(ANIMS_BUILD_DIR)/%.h)
OBJS := $(SRCS:%.c=$(TARGET_BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

all: $(TARGET_BUILD_DIR)/$(TARGET)

$(TARGET_BUILD_DIR)/$(TARGET): animation_headers $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(TARGET_BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(ANIMS_BUILD_DIR)/$(ANIM_ROOT_FILE): $(ANIM_HDRS)
	python $(TOOLS_DIR)/build_slf.py export_root $(ANIMS_DIR) -o $@

$(ANIMS_BUILD_DIR)/%.h: $(ANIMS_DIR)/%
	$(MKDIR_P) $(dir $@)
	python $(TOOLS_DIR)/build_slf.py build $< -o $@

animation_headers: $(ANIMS_BUILD_DIR)/$(ANIM_ROOT_FILE)

clean:
	rm -rf $(TARGET_BUILD_DIR)
	rm -rf $(ANIMS_BUILD_DIR)

distclean:
	rm -rf $(BUILD_DIR)

.PHONY: clean distclean animation_headers

MKDIR_P ?= mkdir -p
