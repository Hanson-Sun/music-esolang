#include <iostream>
#include "../include/MidiReader.h"
#include "../include/Tokenizer.h"

int main() {
    Midi midi = MidiReader::read("../midi/parserTest2.mid");
    Tokenizer tokenizer(midi);
    Tokenizer::TokenizerIterator tokenizerIt = tokenizer.begin();
    while (tokenizerIt) {
        std::cout << (*tokenizerIt).toString() << " ";
        ++tokenizerIt;
    }
    std::cout << std::endl;
    // std::cout << (*tokenizer).toString();
    return 0;
}