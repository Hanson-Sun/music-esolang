#include <iostream>
#include "../include/MidiReader.h"
#include "../include/Tokenizer.h"

int main() {
    Tokenizer tokenizer("../midi/tokentest2.mid");
    Tokenizer::TokenizerIterator tokenizerIt = tokenizer.begin();
    while (tokenizerIt) {
        std::cout << (*tokenizerIt).toString();
        ++tokenizerIt;
    }
    std::cout << std::endl;
    // std::cout << (*tokenizer).toString();
    return 0;
}