TARGET := wsg

SOURCES_FILES := $(wildcard *.c)
SOURCES = $(SOURCES_FILES)
OBJECTS = $(SOURCES:.c=.o)
INCLUDES = $(wildcard *.h)

COMPILER := gcc
COMPILER_FLAGS = -O3 -Werror -I.
LINKER := gcc
LINKER_FLAGS := -lcrypto

.DEFAULT_GOAL := all
.PHONY: all
all: $(TARGET)

.PHONY: debug
debug: COMPILER_FLAGS += -g -DDEBUG
debug: LINKER_FLAGS += -g
debug: clean $(TARGET)

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(TARGET)

%.o: %.c $(INCLUDES)
	$(COMPILER) $(COMPILER_FLAGS) -c -o $@ $<

$(TARGET): $(INCLUDES) $(SOURCES) $(OBJECTS)
	$(LINKER) -o $@ $(OBJECTS) $(LINKER_FLAGS)
