CXX = g++
CXXFLAGS = -std=c++11 -Wall -g
TARGET = circulus
SRC_DIR = src
BIN_DIR = bin
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(SOURCES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -rf $(BIN_DIR)

run: all
	./$(BIN_DIR)/$(TARGET)

.PHONY: all clean run