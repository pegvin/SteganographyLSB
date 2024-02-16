CC:=gcc
C_FLAGS:=--std=c99 -MMD -MP -Wall -Isrc/
LD_FLAGS:=
BUILD_TARGET:=debug

ODIR    := build
BIN     := msghide
SRCS_C  := $(wildcard src/*.c) $(wildcard src/**/*.c)
OBJECTS := $(SRCS_C:.c=.o)
OBJECTS := $(patsubst %,$(ODIR)/%,$(OBJECTS))
DEPENDS := $(OBJECTS:.o=.d)

ifeq ($(BUILD_TARGET),debug)
	C_FLAGS+=-O0 -g
else
	ifeq ($(BUILD_TARGET),release)
		C_FLAGS+=-O3
	else
$(error Invalid Build Target: "$(BUILD_TARGET)")
	endif
endif

ifeq ($(OS),Windows_NT)
	BIN=msghide.exe
else
	# Possible Values Like "Linux" & "Darwin"
	UNAME_S:=$(shell uname -s)

	ifeq ($(UNAME_S),Linux)
		LD_FLAGS+=-ldl

		# On Linux Use Address Sanitizers in Debug Mode
		ifeq ($(BUILD_TARGET),debug)
			C_FLAGS+=-fsanitize=address -fsanitize=undefined
			LD_FLAGS+=-fsanitize=address -fsanitize=undefined
		endif
	endif
endif

-include $(DEPENDS)

$(ODIR)/%.o: %.c
	@echo "CC -" $<
	@mkdir -p "$$(dirname "$@")"
	@$(CC) -c $< -o $@ $(C_FLAGS)

$(BIN): $(OBJECTS)
	@echo Linking $@
	@$(CXX) -o $@ $(OBJECTS) $(LD_FLAGS)

# make all
all: $(BIN)

.PHONY: run
.PHONY: clean

# make run
run: $(BIN)
	@./$(BIN)

# make clean
clean:
	@$(RM) -r $(BIN) $(ODIR) $(DEPENDS)
	@echo - Cleaned

