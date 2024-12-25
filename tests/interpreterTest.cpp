#include "Interpreter.h"
#include "ASTNodes.h"
#include <iostream>

int main() {
    // Create a simple program with a few statements
    Program_t program = std::make_shared<Program>();

    // Create a literal statement
    Token literalToken(TokenType::LITERAL, "lit", 42);
    Statement_t literal = std::make_shared<Literal>(literalToken);

    Token literalToken2(TokenType::LITERAL, "lit", 12);
    Statement_t literal2 = std::make_shared<Literal>(literalToken2);

    // Create an arithmetic operation statement (42 + 42)
    Token addToken(TokenType::ADD, "+");
    Statement_t arithmeticOp = std::make_shared<ArithmeticOp>(addToken);

    Token debugToken(TokenType::DEBUG, "debug");
    Statement_t debug = std::make_shared<IoOp>(debugToken);

    // Add the statements to the program
    program->statements.push_back(literal);
    program->statements.push_back(literal2);
    program->statements.push_back(debug);
    program->statements.push_back(arithmeticOp);
    program->statements.push_back(debug);
    program->statements.push_back(arithmeticOp);
    program->statements.push_back(arithmeticOp);

    // Create an interpreter
    Interpreter interpreter;

    // Execute the program
    auto result = interpreter.visit(*program);

    if (_check(result)) {
        ErrorHandler::printError(std::get<Error>(result));
    }

    return 0;
}