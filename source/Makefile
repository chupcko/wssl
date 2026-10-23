TARGET := libwssl.a
OUT_DIR := ../out
INCLUDE_DIR := ../include

SOURCES := $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
PROTOTYPE = prototype.h
INCLUDES = $(PROTOTYPE) $(wildcard $(INCLUDE_DIR)/*.h)

COMPILER := gcc
COMPILER_FLAGS = -std=c99 -O3 -Werror -I. -I$(INCLUDE_DIR)
ARCHIVER := ar
ARCHIVER_FLAGS := csr

# OpenSSL compile flags, no autoconf used.
# Empty when pkg-config is missing, then the defaults assume
# headers from libssl-dev in /usr/include.
COMPILER_FLAGS += $(shell pkg-config --cflags openssl 2>/dev/null)

.DEFAULT_GOAL := all
.PHONY: all
all: $(OUT_DIR)

.PHONY: debug
debug: COMPILER_FLAGS += -g -DWSSL_DEBUG
debug: all

.PHONY: clean
clean:
	rm -r -f $(PROTOTYPE) $(OBJECTS) $(TARGET) $(OUT_DIR) core

$(OUT_DIR): $(TARGET)
	../tools/make_out
	touch $(OUT_DIR)

$(PROTOTYPE): $(SOURCES)
	../tools/make_prototype

$(TARGET): $(OBJECTS)
	$(ARCHIVER) $(ARCHIVER_FLAGS) $@ $^
	
.SECONDARY:
.SUFFIXES:

%.o: %.c $(INCLUDES)
	$(COMPILER) $(COMPILER_FLAGS) -c -o $@ $<
