#include <iostream>
#include "../include/MidiReader.h"
#include "../include/Tokenizer.h"

int main() {

    Tokenizer tokenizer("../midi/tokentest2.mid");
    Tokenizer::TokenizerIterator tokenizerIt(&tokenizer);
    while (tokenizerIt) {
        std::cout << (*tokenizerIt).toString();
        ++tokenizerIt;
    }
    //std::cout << (*tokenizer).toString();
    return 0;
}