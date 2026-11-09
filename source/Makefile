TARGET_A := libwssl.a
TARGET_SO := libwssl.so
OUT_DIR := ../out
INCLUDE_DIR := ../include

SOURCES := $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
PROTOTYPE = prototype.h
INCLUDES = $(PROTOTYPE) $(wildcard $(INCLUDE_DIR)/*.h)

COMPILER := gcc
COMPILER_FLAGS = -std=c99 -O3 -Werror -fPIC -I. -I$(INCLUDE_DIR) $(shell pkg-config --cflags openssl 2>/dev/null)
ARCHIVER := ar
ARCHIVER_FLAGS := csr
LINKER := gcc
LINKER_FLAGS := -shared -Wl,-soname,$(TARGET_SO)
LIBRARIES := $(or $(shell pkg-config --libs openssl 2>/dev/null),-lssl -lcrypto)

.DEFAULT_GOAL := all
.PHONY: all
all: $(TARGET_A) $(TARGET_SO) $(OUT_DIR)

.PHONY: debug
debug: COMPILER_FLAGS += -g -DWSSL_DEBUG
debug: LINKER_FLAGS += -g
debug: all

.PHONY: clean
clean:
	rm -r -f $(PROTOTYPE) $(OBJECTS) $(TARGET_A) $(TARGET_SO) $(OUT_DIR) core

$(OUT_DIR): $(TARGET_A) $(TARGET_SO)
	../tools/make_out
	touch $(OUT_DIR)

$(PROTOTYPE): $(SOURCES)
	../tools/make_prototype

$(TARGET_A): $(OBJECTS)
	$(ARCHIVER) $(ARCHIVER_FLAGS) $@ $^

$(TARGET_SO): $(OBJECTS)
	$(LINKER) $(LINKER_FLAGS) -o $@ $^ $(LIBRARIES)

.SECONDARY:
.SUFFIXES:

%.o: %.c $(INCLUDES)
	$(COMPILER) $(COMPILER_FLAGS) -c -o $@ $<
