WSSL_DIR := ../out

WSSL_INCLUDE = $(WSSL_DIR)/wssl.h

SOURCES := $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
TARGETS = $(SOURCES:.c=.t)

COMPILER := gcc
COMPILER_FLAGS = -O3 -Werror -I. -I$(WSSL_DIR)
LINKER := gcc
LINKER_FLAGS =
LIBRARIES = $(WSSL_DIR)/libwssl.a

.DEFAULT_GOAL := all
.PHONY: all
all: $(TARGETS)

.PHONY: debug
debug: COMPILER_FLAGS += -g -DDEBUG
debug: LINKER_FLAGS += -g
debug: all

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(TARGETS)

.SECONDARY:
.SUFFIXES:

%.o: %.c $(WSSL_INCLUDE)
	$(COMPILER) $(COMPILER_FLAGS) -c -o $@ $<

%.t: %.o
	$(LINKER) $(LINKER_FLAGS) -o $@ $< $(LIBRARIES)
