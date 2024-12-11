#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <../include/MidiReader.h>

enum TokenType{
    ADD, // +
    SUB, // -
    MUL, // *
    DIV, // /
    MOD, // %
    EQ, // =
    LESS, // <
    GREATER, // >
    AND, // &
    OR, // |
    NOT, // ~
    POP, 
    DUP,
    DUPDOT, // dup.
    SWAP,
    SIZE,
    INPUT,
    PRINT,
    PRINTCHAR, // print- 
    DEBUG,
    DEF,
    IF,
    ELSE,
    WHILE,
    VAR, 
    F, // start of identifier 
    IDENTIFIER, // names of functions & variables ig
    NUMBER, // for literals
    STORE, // !
    LOAD, // @
    FREE, // ^
    END,
    COMMENT,
    DIGIT // solely used for peek()
};

class Token{
    public:
        TokenType type;
        std::string chordLexeme;
        Token() {};
        Token(TokenType type, std::string chordLexeme): type(type), chordLexeme(chordLexeme) {};
        std::string toString() {return chordLexeme+" ";};
};

class Tokenizer {
    public:
        using value_type = Token;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;
        using iterator_category = std::forward_iterator_tag;

        Tokenizer(std::string file);

        reference operator*() {
            return currentToken;
        }

        void reset();

        Tokenizer operator++();
        
        explicit operator bool() {
            return what < 2;
        }
    private:
        int what; // idk either dont delete this
        std::string file;
        MidiReader mr;
        Midi midi;
        MidiTrack::GroupIterator groupIt;
        Token currentToken;
        Token chordToToken();
        Token chordToKeyword();
        Token chordToNumber();
        Token chordToComment();
        Token chordToIdentifier();
        std::string formatCurrentChord();
        std::string pitchToNoteName(int pitch);
        int base12toDecimal(std::vector<std::string> base12);
        TokenType peek();
};
#endif