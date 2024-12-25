#pragma once
#include "Tokenizer.h"
#include "ASTNodes.h"

#include <string>

// ok it does something idk what tho
class Parser {
 public:
    explicit Parser(Tokenizer& tokenizer) : tokenizer(tokenizer), currentToken(tokenizer.begin()), program(std::make_shared<Program>()) {}
    Program_t parse();

    // for the interpreter
    class ParserIterator {
     public:
        using value_type = Statement_t;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;
        using iterator_category = std::forward_iterator_tag;

        explicit ParserIterator(Parser* parser) : parser(parser) {}
        reference operator*() {parser->program->statements[parser->program->statements.size()-1];};
        ParserIterator& operator++() {parser->parse();};

     private:
        Parser* parser;
    };

 private:
    Token consumeToken();
    void expectToken(TokenType receivedType, const std::string& errorMessage);

    Statement_t statement();
    Literal_t literal();
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
    Program_t program;
};
