PAWNCC := ./qawno/pawncc

INCLUDES := \
	./qawno/include \
	./dependencies \

SOURCE := main.p
OUTPUT := main.amx

PAWN_FLAGS := -d3 -Z

export LD_LIBRARY_PATH := $(CURDIR)/qawno

INCLUDE_FLAGS := $(foreach dir,$(INCLUDES),-i$(dir))

.PHONY: all build clean rebuild

all: build

build:
	$(PAWNCC) $(SOURCE) $(INCLUDE_FLAGS) $(PAWN_FLAGS) -o$(OUTPUT)

clean:
	rm -f $(OUTPUT)

rebuild: clean build