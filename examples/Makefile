WSSL_DIR := ../out

WSSL_INCLUDE = $(WSSL_DIR)/wssl.h
WSSL_LIBRARY = $(WSSL_DIR)/libwssl.a

SOURCES := $(wildcard *.c)
INCLUDES := $(wildcard *.h)
OBJECTS = $(SOURCES:.c=.o)
TARGETS = $(SOURCES:.c=.e)

COMPILER := gcc
COMPILER_FLAGS = -std=c99 -O3 -Werror -I. -I$(WSSL_DIR)
LINKER := gcc

# IPv6 example servers: 1 to bind both 0.0.0.0 and ::, 0 for IPv4 only.
# Usage: make IPV6=0
IPV6 ?= 1
COMPILER_FLAGS += -DWSSL_EXAMPLE_IPV6=$(IPV6)
LINKER_FLAGS =
LIBRARIES := $(or $(shell pkg-config --libs openssl 2>/dev/null),-lssl -lcrypto)

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

%.e: %.o $(WSSL_LIBRARY)
	$(LINKER) $(LINKER_FLAGS) -o $@ $< $(WSSL_LIBRARY) $(LIBRARIES)
