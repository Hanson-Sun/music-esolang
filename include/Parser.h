#pragma once
#include "Tokenizer.h"
#include "ASTNodes.h"

// ok it does something idk what tho
class Parser {
 public:
    Parser(Tokenizer& tokenizer) : tokenizer(tokenizer), currentToken(tokenizer.begin()) {}
    Program* parse();

 private:
    Token consumeToken();
    void expectToken(TokenType expectedType, const std::string& errorMessage);

    Program* program();
    Statement* statement();
    Literal* literal();
    Identifier* identifier();
    IdentifierCall* identifierCall();
    ArithmeticOp* arithmeticOp();
    LogicalOp* logicalOp();
    StackOp* stackOp();
    IoOp* ioOp();
    ControlFlow* controlFlow();
    IfElse* ifElse();
    While* whileStatement();
    Block* block();
    VariableOp* variableOp();
    Definition* definition();
    Comment* comment();

 private:
    Tokenizer& tokenizer;
    Tokenizer::TokenizerIterator currentToken;
};