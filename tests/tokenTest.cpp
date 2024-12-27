#include <iostream>
#include "../include/MidiReader.h"
#include "../include/Tokenizer.h"

int main() {
    Midi midi = MidiReader::read("../midi/tokentest2.mid");
    Tokenizer tokenizer(*midi.begin());
    Tokenizer::TokenizerIterator tokenizerIt = tokenizer.begin();
    while (tokenizerIt) {
        std::cout << (*tokenizerIt).toString() << " ";
        ++tokenizerIt;
    }
    std::cout << std::endl;
    return 0;
}