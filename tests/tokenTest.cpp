#include <iostream>
#include "../include/MidiReader.h"
#include "../include/Tokenizer.h"

int main() {
    Tokenizer tokenizer("../midi/chordTest.mid");
    while (tokenizer) {
        std::cout << tokenizer.currentToken.toString();
        ++tokenizer;
    }
    return 0;
}