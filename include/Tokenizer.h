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
    LOAD, // @a
    FREE, // ^
    END,
    COMMENT
};

class Token{
    public:
        TokenType type;
        std::string chordLexeme;
        Token(TokenType type, std::string chordLexeme): type(type), chordLexeme(chordLexeme) {};
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
};

class Tokenizer {
    public:
        Tokenizer(std::string file): file(file) {};
        bool tokenize();
    private:
        std::string file;
        std::vector<std::string> tokens;
        Token chordToToken(std::string chord);
};