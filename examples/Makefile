WSSL_DIR := ../out

WSSL_INCLUDE = $(WSSL_DIR)/wssl.h

SOURCES := $(wildcard *.c)
INCLUDES := $(wildcard *.h)
OBJECTS = $(SOURCES:.c=.o)
TARGETS = $(SOURCES:.c=.e)

COMPILER := gcc
COMPILER_FLAGS = -std=c99 -O3 -Werror -I. -I$(WSSL_DIR)
LINKER := gcc
LINKER_FLAGS =
LIBRARIES = $(WSSL_DIR)/libwssl.a

# OpenSSL link flags, no autoconf used.
# If pkg-config exists it provides the flags, otherwise fall
# back to -lssl -lcrypto (libssl-dev installs to /usr/include).
OPENSSL_LIBS := $(or $(shell pkg-config --libs openssl 2>/dev/null),-lssl -lcrypto)
# NOTE: libraries must stay after objects on the link line,
# the linker drops libraries listed before objects (--as-needed).

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

%.e: %.o
	$(LINKER) $(LINKER_FLAGS) -o $@ $< $(LIBRARIES) $(OPENSSL_LIBS)
