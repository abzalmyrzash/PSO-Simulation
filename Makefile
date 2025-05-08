CXX := g++
CXXFLAGS := -std=c++11 -g -w -m64
# LDFLAGS := -Llib
LDLIBS := -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

# Thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)
TARGET_EXEC := Main

BUILD_DIR := ./build
SRC_DIR := ./src
INC_DIRS := ./include ./include/SDL2

# Find all the C++ files we want to compile
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

# Prepends BUILD_DIR and appends .o to every src file
# As an example, ./src/hello.cpp turns into ./build/hello.o
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.o turns into ./build/hello.d
DEPS := $(OBJS:.o=.d)

# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS := $(INC_FLAGS) -MMD -MP

# The final build step.
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

# Build step for C++ source
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

-include $(DEPS)
