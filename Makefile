CXX = g++
CXX_FLAGS = -Wall -Wextra -std=c++17 -O2
SRC_DIR = src
BUILD_DIR = build
# BUILD_NAME = sparkito
BUILD_NAME = $(BUILD_DIR)/sparkito

INCLUDE_DIRS =  -I $(SRC_DIR)/Math \
				-I $(SRC_DIR)/Util \
				-I $(SRC_DIR)/Geometries

# Find all cpp files in src directory
SOURCES = $(wildcard $(SRC_DIR)/*.cpp) 

# Replace .cpp with .o for object files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

all: $(BUILD_NAME)

# Rule for linking the final executable
# Depends on all object files
$(BUILD_NAME): $(OBJECTS)
	$(CXX) $(CXX_FLAGS) $(INCLUDE_DIRS) -o $@ $^

# Rule for compiling source files to object files
# Each object file depends on the corresponding source file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXX_FLAGS) $(INCLUDE_DIRS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_NAME)

.PHONY: run
run: $(BUILD_NAME)
	./$(BUILD_NAME) > $(BUILD_DIR)/image.ppm
