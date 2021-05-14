CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS = -lpthread

BUILD_DIR = ./build

TARGET = ContarPalabras
TEST_TARGET = UnitTests

OBJECTS = HashMapConcurrente.o CargarArchivos.o

all: build $(BUILD_DIR)/$(TARGET)

test: build $(BUILD_DIR)/$(TEST_TARGET)
	$(BUILD_DIR)/$(TEST_TARGET)

$(BUILD_DIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LDFLAGS)

$(BUILD_DIR)/%: src/%.cpp $(OBJECTS:%=$(BUILD_DIR)/%)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: all test build clean debug

build:
	@mkdir -p $(BUILD_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

clean:
	-@rm -rvf $(BUILD_DIR)/*
