WSSL_DIR := ../out

WSSL_INCLUDE = $(WSSL_DIR)/wssl.h

SOURCES := $(wildcard *.c)
INCLUDES := $(wildcard *.h)
OBJECTS = $(SOURCES:.c=.o)
TARGETS = $(SOURCES:.c=.t)

COMPILER := gcc
COMPILER_FLAGS = -std=c99 -O3 -Werror -I. -I$(WSSL_DIR)
LINKER := gcc
LINKER_FLAGS =
LIBRARIES = $(WSSL_DIR)/libwssl.a

.DEFAULT_GOAL := all
.PHONY: all
all: $(TARGETS)

.PHONY: debug
debug: COMPILER_FLAGS += -g -DWSSL_DEBUG
debug: LINKER_FLAGS += -g
debug: all

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(TARGETS) core

.SECONDARY:
.SUFFIXES:

%.o: %.c $(WSSL_INCLUDE) $(INCLUDES)
	$(COMPILER) $(COMPILER_FLAGS) -c -o $@ $<

%.t: %.o
	$(LINKER) $(LINKER_FLAGS) -o $@ $< $(LIBRARIES)
