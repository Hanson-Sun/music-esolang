#include <iostream>
#include "../include/MidiReader.h"
#include "../include/Tokenizer.h"

int main() {

    Tokenizer tokenizer("../midi/tokentest1.mid");
    while (tokenizer) {
        std::cout << (*tokenizer).toString();
        ++tokenizer;
    }
    return 0;
}