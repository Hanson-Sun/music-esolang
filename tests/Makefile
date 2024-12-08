CXX = g++

CXXFLAGS = -Wall -Wextra -std=c++17 -I../include -I./

SRCS1 = ../src/MidiReader.cpp midiTest.cpp

OBJS1 = $(SRCS1:.cpp=.o)

TARGETS = midiTest

all: $(TARGETS)

midiTest: $(OBJS1)
	$(CXX) $(CXXFLAGS) -o midiTest $(OBJS1)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS1) $(TARGETS)

.PHONY: all clean