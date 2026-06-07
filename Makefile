CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra
LDFLAGS =

BUILD_DIR = build
TARGET = $(BUILD_DIR)/kyklos

SRCS = main.cpp \
       src/core/core.cpp \
       src/cli/cli.cpp \

HEADERS = $(shell find . -name "*.hpp")

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR)

run: all
	./$(TARGET)

.PHONY: all clean run