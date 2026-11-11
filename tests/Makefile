WSSL_DIR := ../out

WSSL_INCLUDE = $(WSSL_DIR)/wssl.h
WSSL_LIBRARY = $(WSSL_DIR)/libwssl.a

SOURCES := $(wildcard *_server.c)
TARGETS = $(SOURCES:.c=.e)

COMPILER := gcc
COMPILER_FLAGS = -std=c99 -O3 -Werror -I$(WSSL_DIR)
LINKER := gcc
LINKER_FLAGS =
LIBRARIES := $(or $(shell pkg-config --libs openssl 2>/dev/null),-lssl -lcrypto)

.DEFAULT_GOAL := all
.PHONY: all
all: $(WSSL_LIBRARY) $(TARGETS)

$(WSSL_LIBRARY):
	$(MAKE) -C ..

PYTHON ?= python3

.PHONY: test
test: all
	$(PYTHON) -m pytest . -q

.PHONY: clean
clean:
	rm -f $(TARGETS) core

.SECONDARY:
.SUFFIXES:

%.e: %.c $(WSSL_INCLUDE)
	$(COMPILER) $(COMPILER_FLAGS) -o $@ $< $(WSSL_LIBRARY) $(LIBRARIES)
