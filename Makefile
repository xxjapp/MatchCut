# Specify configuration like:
# make CFG=debug
# make CFG=release
#
# Make "release" the default configuration if invoked with just "make"
ifeq ($(CFG), )
    CFG=release
endif

# include directories
INC := \
    -I./include \

# libs
LIB := \

# Use g++44 for old centos 5.6
ifneq ("$(wildcard /usr/bin/g++44)","")
    CC      := g++44
else
    CC      := g++
endif

CFLAGS      := -Wall -Wextra -Wno-missing-field-initializers -std=c++0x -D_FILE_OFFSET_BITS=64 $(INC)
LDFLAGS     := $(LIB)

# Separate compile options per configuration
ifeq ($(CFG), debug)
    CFLAGS  += -g3 -D_DEBUG
else
    CFLAGS  += -O3 -D_RELEASE
endif

SRC         := src
OBJ         := obj
BIN         := bin

SRC_DIR     := $(SRC)
OBJ_DIR     := $(OBJ)/$(CFG)
BIN_DIR     := $(BIN)/$(CFG)
SOURCES     := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS     := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))
EXECUTABLE  := $(BIN_DIR)/MatchCut

# Rules for make

# default
all: $(EXECUTABLE)

# link
$(EXECUTABLE): $(OBJECTS)
	@ mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

# pull in dependency info for *existing* .o files
-include $(OBJECTS:.o=.d)

# compile and generate dependency info
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@ mkdir -p $(OBJ_DIR)
	$(CC) -c $(CFLAGS) $< -o $@
	@ echo -n $(OBJ_DIR)/ > $(@:.o=.d)
	@ $(CC) -MM $(CFLAGS) $< >> $(@:.o=.d)

# clean
clean:
	rm -rf $(OBJ) $(BIN)
