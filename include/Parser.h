#pragma once
#include "Tokenizer.h"
#include "ASTNodes.h"

#include <string>

// ok it does something idk what tho
class Parser {
 public:
    explicit Parser(Tokenizer& tokenizer) : tokenizer(tokenizer), currentToken(tokenizer.begin()), program(std::make_shared<Program>()) {}
    Program_t parse();
    Program_t program;

    // for the interpreter
    class ParserIterator {
     public:
        using value_type = Statement_t;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;
        using iterator_category = std::forward_iterator_tag;

        explicit ParserIterator(Parser* parser) : parser(parser) {}
        reference operator*() {return parser->program->statements[parser->program->statements.size()-1];};
        ParserIterator& operator++() {parser->parse(); return *this;};
        explicit operator bool() {return static_cast<bool>(parser->currentToken);};

     private:
        Parser* parser;
    };

   ParserIterator begin() { return ParserIterator(this); }
   ParserIterator end() { return ParserIterator(nullptr); }

 private:
    Token consumeToken();
    void expectToken(TokenType receivedType, const std::string& errorMessage);

    Statement_t statement();
    Literal_t literal();
    Literal_t identifier();
    IdentifierCall_t identifierCall();
    ArithmeticOp_t arithmeticOp();
    LogicalOp_t logicalOp();
    StackOp_t stackOp();
    IoOp_t ioOp();
    IfElse_t ifElse();
    While_t whileStatement();
    VariableOp_t variableOp();
    VariableDeclaration_t variableDeclaration();
    Definition_t definition();

 private:
    Tokenizer& tokenizer;
    Tokenizer::TokenizerIterator currentToken;
};
