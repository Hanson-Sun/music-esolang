CXX = g++

CXXFLAGS = -Wall -Wextra -std=c++17 -I../include -I./

SRCS1 = midiTest.cpp ../src/MidiReader.cpp
SRCS2 = tokenTest.cpp ../src/Tokenizer.cpp

OBJS1 = $(SRCS1:.cpp=.o)
OBJS2 = $(SRCS2:.cpp=.o)

TARGETS = midiTest

all: $(TARGETS)

midiTest: $(OBJS1)
	$(CXX) $(CXXFLAGS) -o midiTest $(OBJS1)  

tokenTest: $(OBJS2)
	$(CXX) $(CXXFLAGS) -o tokenTest $(OBJS2)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS1) $(TARGETS)

.PHONY: all clean