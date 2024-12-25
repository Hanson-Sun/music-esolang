#include <iostream>
#include "../include/Parser.h"
#include "../include/Tokenizer.h"
#include "../include/ASTVisitor.h"

int main() {
    Midi midi = MidiReader::read("../midi/parserTest1.mid");
    Tokenizer tokenizer(midi);

    Parser parser(tokenizer);
    Parser::ParserIterator it = parser.begin();
    while(it) {
        ++it;
    }
    ASTPrinter printer;
    parser.program->accept(printer);
    return 0;
}