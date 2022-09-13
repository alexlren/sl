TARGET := sl
VERSION ?= 0.0.0
ITERATION ?= 0
MAINTAINER := Alex Laurent <alex@i42.sh>
ARCH ?= x86_64
DESCRIPTION := A cruel program made to punish users who mispell ls. Instead of showing the the contents of the current directory as one would expect from ls, sl displays a loooooong random animation crossing the terminal.
PKG_DEB = $(TARGET)_$(VERSION)-$(ITERATION)_$(ARCH).deb
PKG_RPM = $(TARGET)-$(VERSION)-$(ITERATION).$(ARCH).rpm
TMP_INSTALL_DIR ?= /tmp/$(TARGET)-INSTALL-$(VERSION)-$(ITERATION)
FPM_OPTS = -s dir \
	-n $(TARGET) \
	-v $(VERSION) \
	--iteration $(ITERATION) \
	-C $(TMP_INSTALL_DIR) \
	--prefix /usr \
	--maintainer "$(MAINTAINER)" \
	--description "$(DESCRIPTION)" \
	-a $(ARCH) \
	--depends ncurses

BUILD_DIR ?= .build
SRC_DIR := src
ANIMS_DIR := animations
ANIMS_BUILD_DIR := $(SRC_DIR)/generated
ANIM_ROOT_FILE := anims.h
TOOLS_DIR := tools
DESTDIR ?= /usr/local

CC := gcc
CFLAGS := -Wall -Werror -I$(SRC_DIR)
LDFLAGS := -lncurses
DEBUG_CFLAGS := -O0 -g -ggdb
RELEASE_CFLAG := -O2 -DNDEBUG
SRCS := sl.c
HDRS := animation.h train.h

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

ARCHIVE := $(TARGET_BUILD_DIR)/$(TARGET)-$(VERSION).tar.gz

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

install: RELEASE=1
install: $(TARGET_BUILD_DIR)/$(TARGET)
	$(MKDIR_P) $(DESTDIR)/bin
	cp $(TARGET_BUILD_DIR)/$(TARGET) $(DESTDIR)/bin

$(ARCHIVE): RELEASE=1
$(ARCHIVE): $(TARGET_BUILD_DIR)/$(TARGET)
	tar -C $(TARGET_BUILD_DIR) -czvf $@ $(TARGET)

rpm: DESTDIR=$TMP_INSTALLDIR
rpm:
	@rm -rf $(TMP_INSTALL_DIR)
	make install DESTDIR=$(TMP_INSTALL_DIR)
	fpm -s dir -t rpm \
		-p "$(PKG_RPM)" \
		$(FPM_OPTS)

deb: DESTDIR=$TMP_INSTALLDIR
deb:
	@rm -rf $(TMP_INSTALL_DIR)
	make install DESTDIR=$(TMP_INSTALL_DIR)
	fpm -s dir -t deb \
		-p "$(PKG_DEB)" \
		$(FPM_OPTS)

dist: $(ARCHIVE)

clean:
	rm -rf $(TARGET_BUILD_DIR)
	rm -rf $(ANIMS_BUILD_DIR)

distclean: clean
	rm -rf $(BUILD_DIR)

.PHONY: clean distclean animation_headers dist install

MKDIR_P ?= mkdir -p
