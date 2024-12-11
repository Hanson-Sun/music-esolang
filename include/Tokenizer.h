#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "MidiReader.h"

enum TokenType {
    ADD,      // +
    SUB,      // -
    MUL,      // *
    DIV,      // /
    MOD,      // %
    EQ,       // =
    LESS,     // <
    GREATER,  // >
    AND,      // &
    OR,       // |
    NOT,      // ~
    POP,
    DUP,
    DUPDOT,  // dup.
    SWAP,
    SIZE,
    INPUT,
    PRINT,
    PRINTCHAR,  // print-
    DEBUG,
    DEF,
    IF,
    ELSE,
    WHILE,
    VAR,
    F,        // start of identifier
    LITERAL,  // for literals (numbers & identifiers)
    STORE,    // !
    LOAD,     // @
    FREE,     // ^
    END,
    COMMENT,
    DIGIT  // solely used for peek()
};

class Token {
 public:
    TokenType type;
    std::string chordLexeme;
    Token() {}
    Token(TokenType type, std::string chordLexeme) : type(type), chordLexeme(chordLexeme) {}
    std::string toString() { return chordLexeme + " "; }
};

class Tokenizer {
 public:
    class TokenizerIterator {
     public:
        using value_type = Token;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;
        using iterator_category = std::forward_iterator_tag;

        explicit TokenizerIterator(Tokenizer* tokenizer) : tokenizer(tokenizer), what(0) {}

        reference operator*() { return tokenizer->currentToken; }

        TokenizerIterator operator++();

        explicit operator bool() { return what < 2; }

     private:
        Tokenizer* tokenizer;
        int what;  // idk either dont delete this
    };
    friend class TokenizerIterator;
    explicit Tokenizer(std::string file);

 private:
    std::string file;
    MidiReader mr;
    Midi midi;
    MidiTrack::GroupIterator groupIt;
    Token currentToken;
    Token chordToToken();
    Token chordToKeyword();
    Token chordToLiteral();
    Token chordToComment();
    std::string formatCurrentChord();
    std::string pitchToNoteName(int pitch);
    int base12toDecimal(std::vector<std::string> base12);
    TokenType peek();
};
