TARGET := libwssl.a
INCLUDE_DIR := ../include

SOURCES := $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
PROTOTYPE := $(INCLUDE_DIR)/prototype.h
INCLUDES = $(filter-out $(PROTOTYPE), $(wildcard $(INCLUDE_DIR)/*.h)) $(PROTOTYPE)

COMPILER := gcc
COMPILER_FLAGS = -O3 -Werror -I. -I$(INCLUDE_DIR)
ARCHIVER = ar
ARCHIVER_FLAGS = csr


.DEFAULT_GOAL := all
.PHONY: all
all: $(TARGET)
	../tool/make_out

.PHONY: debug
debug: COMPILER_FLAGS += -g
debug: all

.PHONY: clean
clean:
	rm -f $(PROTOTYPE) $(OBJECTS) $(TARGET)

$(PROTOTYPE): $(SOURCES)
	../tool/update_prototype

$(TARGET): $(OBJECTS)
	$(ARCHIVER) $(ARCHIVER_FLAGS) $@ $^
	
.SUFFIXES:
%.o: %.c $(INCLUDES)
	$(COMPILER) $(COMPILER_FLAGS) -c -o $@ $<
