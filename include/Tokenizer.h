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

std::unordered_map<std::string, TokenType> CHORDTOKENMAP = {
    {"(5)", TokenType::DEF},
    {"(5, 4)", TokenType::END},
    {"(13)", TokenType::COMMENT},
    {"(4)", TokenType::F},
    {"(8, 2)", TokenType::ADD},
    {"(8, 3)", TokenType::SUB},
    {"(8, 4)", TokenType::MUL},
    {"(8, 5)", TokenType::DIV},
    {"(8, 6)", TokenType::MOD},
    {"(7, 2)", TokenType::EQ},
    {"(7, 3)", TokenType::LESS},
    {"(7, 4)", TokenType::GREATER},
    {"(6, 2)", TokenType::AND},
    {"(6, 3)", TokenType::OR},
    {"(6, 4)", TokenType::NOT},
    {"(9, 2)", TokenType::POP},
    {"(9, 3)", TokenType::DUP},
    {"(9, 4)", TokenType::DUPDOT},
    {"(9, 5)", TokenType::SWAP},
    {"(9, 6)", TokenType::SIZE},
    {"(5, 4, 4)", TokenType::INPUT},
    {"(5, 4, 5)", TokenType::PRINT},
    {"(5, 4, 6)", TokenType::PRINTCHAR},
    {"(5, 4, 7)", TokenType::DEBUG},
    {"(5, 5, 4)", TokenType::IF},
    {"(5, 5, 5)", TokenType::ELSE},
    {"(5, 5, 6)", TokenType::WHILE},
    {"(8)", TokenType::VAR},
    {"(9)", TokenType::STORE},
    {"(10)", TokenType::LOAD},
    {"(11)", TokenType::FREE},
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

        Tokenizer(std::string file) : file(file) {};

        reference operator*() {
            return currentToken;
        }

        void reset();

        Tokenizer operator++();
        
        explicit operator bool() {
            return *groupIt != midi.begin()->group_end();
        }
    private:
        std::string file;
        MidiReader mr;
        Midi midi;
        MidiTrack::GroupIterator *groupIt;
        Token currentToken;
        Token chordToToken();
        Token chordToKeyword();
        Token chordToNumber();
        Token chordToIdentifier();
        std::string formatCurrentChord();
        std::string pitchToNoteName(int pitch);
        int base12toDecimal(std::vector<std::string> base12);
        TokenType peek();
};
#endif