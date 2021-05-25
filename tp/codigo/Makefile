CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS = -lpthread

BUILD_DIR = ./build

TARGET = ContarPalabras
TEST_TARGET = UnitTests
EXPERIMENTOS_TARGET = Experimentos

OBJECTS = HashMapConcurrente.o CargarArchivos.o

all: build $(BUILD_DIR)/$(TARGET)

experimentos: build $(BUILD_DIR)/$(EXPERIMENTOS_TARGET)

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
debug: test#all

clean:
	-@rm -rvf $(BUILD_DIR)/*
