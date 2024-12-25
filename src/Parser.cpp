#include "Parser.h"
#include <iostream>

Token Parser::consumeToken() {
    ++currentToken;
    return *currentToken;
}

void Parser::expectToken(TokenType receivedType, const std::string& errorMessage) {
    std::cerr << errorMessage << ". Received: " << receivedType << std::endl;
    // TODO: maybe want exceptions here?
}

Program_t Parser::parse() {
    if (currentToken) {
        Statement_t statement = this->statement();
        if (statement) {
            program->statements.push_back(statement);
        }
    }
    return program;
}

Statement_t Parser::statement() {
    switch ((*currentToken).type) {
        case COMMENT:
            return nullptr;
        case LITERAL:
            return literal();
        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case MOD:
            return arithmeticOp();
        case EQ:
        case LESS:
        case GREATER:
        case AND:
        case OR:
        case NOT:
            return logicalOp();
        case POP:
        case DUP:
        case DUPDOT:
        case SWAP:
        case SIZE:
            return stackOp();
        case INPUT:
        case PRINT:
        case PRINTCHAR:
        case DEBUG:
            return ioOp();
        case IF:
            return ifElse();
        case WHILE:
            return whileStatement();
        case F:
            return identifierCall();
        case VAR:
            return variableDeclaration();
        case STORE:
        case LOAD:
        case FREE:
            return variableOp();
        case DEF:
            return definition();
        default:
            expectToken((*currentToken).type, "Expected a statement");
            return nullptr;
    }
}

Literal_t Parser::literal() {
    Literal_t temp = std::make_shared<Literal>(*currentToken);
    consumeToken();
    // optional end token
    if ((*currentToken).type == END) {
        consumeToken();
    }
    return temp;
}

ArithmeticOp_t Parser::arithmeticOp() {
    ArithmeticOp_t temp = std::make_shared<ArithmeticOp>(*currentToken);
    consumeToken();
    return temp;
}

LogicalOp_t Parser::logicalOp() {
    LogicalOp_t temp = std::make_shared<LogicalOp>(*currentToken);
    consumeToken();
    return temp;
}

StackOp_t Parser::stackOp() {
    StackOp_t temp = std::make_shared<StackOp>(*currentToken);
    consumeToken();
    return temp;
}

IoOp_t Parser::ioOp() {
    IoOp_t temp = std::make_shared<IoOp>(*currentToken);
    consumeToken();
    return temp;
}

IfElse_t Parser::ifElse() {
    consumeToken();  // consume "if"
    Block_t thenBranch = std::make_shared<Block>();
    while ((*currentToken).type != ELSE && (*currentToken).type != END && currentToken) {
        thenBranch->statements.push_back(statement());
    }
    Block_t elseBranch = nullptr;
    if ((*currentToken).type == ELSE) {
        consumeToken();  // consume "else"
        elseBranch = std::make_shared<Block>();
        while ((*currentToken).type != END && currentToken) {
            elseBranch->statements.push_back(statement());
        }
    }
    if ((*currentToken).type == END) {
        consumeToken();  // consume "end"
    } else {
        //error stop building syntax tree
        expectToken((*currentToken).type, "Expected 'end' after if-else statement");
        return nullptr;
    }
    return std::make_shared<IfElse>(thenBranch, elseBranch);
}

While_t Parser::whileStatement() {
    consumeToken();  // consume "while"
    Block_t body = std::make_shared<Block>();
    while ((*currentToken).type != END && currentToken) {
        body->statements.push_back(statement());
    }
    if ((*currentToken).type == END) {
            consumeToken();  // consume "end"
    } else {    
        //error stop building syntax tree
        expectToken((*currentToken).type, "Expected 'end' after while statement");
        return nullptr;
    }
    return std::make_shared<While>(body);
}

IdentifierCall_t Parser::identifierCall() {
    consumeToken();  // consume "f"
    Literal_t id = literal();
    if ((*currentToken).type == END) {
        consumeToken();  // consume "end"
    } else {
        //error stop building syntax tree
        expectToken((*currentToken).type, "Expected 'end' after identifier call");
        return nullptr;
    }
    return std::make_shared<IdentifierCall>(id);
}

VariableDeclaration_t Parser::variableDeclaration() {
    consumeToken();  // consume "var"
    Literal_t id = literal();
    if ((*currentToken).type == END) {
        consumeToken();  // consume "end"
    } else {
        //error stop building syntax tree
        expectToken((*currentToken).type, "Expected 'end' after variable declaration");
        return nullptr;
    }
    return std::make_shared<VariableDeclaration>(id);
}

VariableOp_t Parser::variableOp() {
    VariableOp_t temp = std::make_shared<VariableOp>(*currentToken);
    consumeToken();
    return temp;
}

Definition_t Parser::definition() {
    consumeToken();  // consume "def"
    Literal_t id = literal();
    Block_t body = std::make_shared<Block>();
    while ((*currentToken).type != END && currentToken) {
        body->statements.push_back(statement());
    }
    if ((*currentToken).type == END) {
        consumeToken();  // consume "end"
    } else {
        //error stop building syntax tree
        expectToken((*currentToken).type, "Expected 'end' after definition");
        return nullptr;
    }
    return std::make_shared<Definition>(id, body);
}




