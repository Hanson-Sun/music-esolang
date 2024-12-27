#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <variant>

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
    SPACE,
    COMMENT,
    DIGIT  // solely used for peek()
};

class Token {
 public:
    TokenType type;
    std::string chordLexeme;
    int64_t value;
    Token() : type(TokenType::END), chordLexeme(""), value(0) {}
    Token(TokenType type, const std::string& chordLexeme, int value = 0) : type(type), chordLexeme(chordLexeme), value(value) {}
    std::string toString() const { 
        if (type == TokenType::LITERAL)
           return  "Literal(" + std::to_string(value) + ")"; 
        return chordLexeme;
    }

};

class Tokenizer {
 public:
    explicit Tokenizer(MidiTrack midi);
    explicit Tokenizer(std::vector<MidiNote> notes);

    class TokenizerIterator {
     private:
        Tokenizer* tokenizer;
        int what;  // idk either dont delete this
     public:
        using value_type = Token;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;
        using iterator_category = std::forward_iterator_tag;

        explicit TokenizerIterator(Tokenizer* tokenizer) : tokenizer(tokenizer), what(0) {}

        reference operator*() { return tokenizer->currentToken; }
        TokenizerIterator& operator++();
        bool operator!=(const TokenizerIterator& other) const {
            return tokenizer != other.tokenizer;
        }
        explicit operator bool() { return what < 2; }
    };

    TokenizerIterator begin() { return TokenizerIterator(this); }
    TokenizerIterator end() { return TokenizerIterator(nullptr); }

    std::vector<Token> tokenize();

 private:
    MidiTrack midi;
    MidiTrack::GroupIterator groupIt;
    Token currentToken;
    Token chordToToken();
    Token chordToKeyword();
    Token chordToLiteral();
    Token chordToComment();
    std::string formatCurrentChord();
    std::string pitchToNoteName(int pitch);
    int64_t base12toDecimal(const std::vector<std::string>& base12);
    TokenType peek();

};
