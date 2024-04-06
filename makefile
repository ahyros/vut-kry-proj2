# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 #-fsanitize=address -fsanitize=leak

# Source files
SRCS = main.cpp argParser.cpp sha256.cpp utils.cpp error.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

# Executable
TARGET = kry

# Targets
.PHONY: all clean

all: $(TARGET) clean_objs

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean_objs:
	rm $(OBJS) $(DEPS)

clean:
	rm $(OBJS) $(DEPS) $(TARGET)

-include $(DEPS)
