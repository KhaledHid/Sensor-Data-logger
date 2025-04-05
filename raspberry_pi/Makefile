# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Iinc

# Source and output
SRCS = $(wildcard src/*.cpp)
OUT = build/logger

# Default build rule
all: $(OUT)

$(OUT): $(SRCS)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(OUT)

# Clean build
clean:
	rm -f $(OUT)
