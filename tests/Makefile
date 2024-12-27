CXX = g++

CXXFLAGS = -Wall -Wextra -std=c++17 -I../include -I./

SRCS1 = midiTest.cpp ../src/MidiReader.cpp
SRCS2 = tokenTest.cpp ../src/Tokenizer.cpp ../src/MidiReader.cpp
SRCS3 = interpreterTest.cpp ../src/Interpreter.cpp
SRCS4 = parserTest.cpp ../src/Parser.cpp ../src/Tokenizer.cpp ../src/MidiReader.cpp ../src/ASTPrinter.cpp

OBJS1 = $(SRCS1:.cpp=.o)
OBJS2 = $(SRCS2:.cpp=.o) 
OBJS3 = $(SRCS3:.cpp=.o)
OBJS4 = $(SRCS4:.cpp=.o)

TARGETS = midiTest tokenTest interpreterTest parserTest

all: $(TARGETS)

midiTest: $(OBJS1)
	$(CXX) $(CXXFLAGS) -o midiTest $(OBJS1)  

tokenTest: $(OBJS2)
	$(CXX) $(CXXFLAGS) -o tokenTest $(OBJS2)

interpreterTest: $(OBJS3)
	$(CXX) $(CXXFLAGS) -o interpreterTest $(OBJS3)

parserTest: $(OBJS4)
	$(CXX) $(CXXFLAGS) -o parserTest $(OBJS4)


%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4) $(TARGETS)

.PHONY: all clean