#include <iostream>
#include "../include/MidiReader.h"
#include "../include/Tokenizer.h"

int main() {

    Tokenizer tokenizer("../midi/tokentest2.mid");
    while (tokenizer) {
        std::cout << (*tokenizer).toString();
        ++tokenizer;
    }
    //std::cout << (*tokenizer).toString();
    return 0;
}