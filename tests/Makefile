WSSL_DIR := ../out

WSSL_INCLUDE = $(WSSL_DIR)/wssl.h
WSSL_LIBRARY = $(WSSL_DIR)/libwssl.a

SOURCES := $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
TARGETS = $(SOURCES:.c=.e)

COMPILER := gcc
COMPILER_FLAGS = -std=c99 -O3 -Werror -I$(WSSL_DIR)
LINKER := gcc
LINKER_FLAGS =
LIBRARIES := $(or $(shell pkg-config --libs openssl 2>/dev/null),-lssl -lcrypto)

.DEFAULT_GOAL := all
.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(TARGETS) core

.PHONY: test
test: all
	python3 -m pytest . -v

.SECONDARY:
.SUFFIXES:

%.o: %.c $(WSSL_INCLUDE) $(INCLUDES)
	$(COMPILER) $(COMPILER_FLAGS) -c -o $@ $<

%.e: %.o $(WSSL_LIBRARY)
	$(LINKER) $(LINKER_FLAGS) -o $@ $< $(WSSL_LIBRARY) $(LIBRARIES)
