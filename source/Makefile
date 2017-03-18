TARGET := libwssl.a
OUT_DIR := ../out
INCLUDE_DIR := ../include

SOURCES := $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
PROTOTYPE = $(INCLUDE_DIR)/prototype.h
INCLUDES = $(filter-out $(PROTOTYPE), $(wildcard $(INCLUDE_DIR)/*.h)) $(PROTOTYPE)

COMPILER := gcc
COMPILER_FLAGS = -O3 -Werror -I. -I$(INCLUDE_DIR)
ARCHIVER := ar
ARCHIVER_FLAGS := csr

.DEFAULT_GOAL := all
.PHONY: all
all: $(OUT_DIR)

.PHONY: debug
debug: COMPILER_FLAGS += -g -DDEBUG
debug: all

.PHONY: clean
clean:
	rm -r -f $(PROTOTYPE) $(OBJECTS) $(TARGET) $(OUT_DIR) core

$(OUT_DIR): $(TARGET)
	../tool/make_out
	touch $(OUT_DIR)

$(PROTOTYPE): $(SOURCES)
	../tool/update_prototype

$(TARGET): $(OBJECTS)
	$(ARCHIVER) $(ARCHIVER_FLAGS) $@ $^
	
.SECONDARY:
.SUFFIXES:

%.o: %.c $(INCLUDES)
	$(COMPILER) $(COMPILER_FLAGS) -c -o $@ $<
