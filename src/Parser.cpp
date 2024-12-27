#include "Parser.h"
#include <iostream>

Token Parser::consumeToken() {
    ++currentToken;
    return *currentToken;
}

_<Statement_t> Parser::parse() {
    if (currentToken) {
        auto statement = this->statement();
        if (_check(statement)) 
            return ErrorHandler::addContext(statement, "Failed to parse statement: " + (*currentToken).toString());
        program->statements.push_back(std::get<Statement_t>(statement));
        return statement;
    }
    return ErrorHandler::createError(ErrorCode::INVALID_STATEMENT, "Parsing token error");
}

_<Statement_t> Parser::statement() {
    switch ((*currentToken).type) {
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
            return ErrorHandler::createError(ErrorCode::INVALID_TOKEN, "Invalid token type: " + (*currentToken).toString());
    }
}

_<Statement_t> Parser::literal() {
    Literal_t temp = std::make_shared<Literal>(*currentToken);
    consumeToken();
    // optional space token
        if ((*currentToken).type == SPACE) {
            consumeToken();
        }
    return temp;
}

_<Statement_t> Parser::identifier() {
    Literal_t temp = std::make_shared<Literal>(*currentToken);
    consumeToken();
    return temp;
}

_<Statement_t> Parser::arithmeticOp() {
    ArithmeticOp_t temp = std::make_shared<ArithmeticOp>(*currentToken);
    consumeToken();
    return temp;
}

_<Statement_t> Parser::logicalOp() {
    LogicalOp_t temp = std::make_shared<LogicalOp>(*currentToken);
    consumeToken();
    return temp;
}

_<Statement_t> Parser::stackOp() {
    StackOp_t temp = std::make_shared<StackOp>(*currentToken);
    consumeToken();
    return temp;
}

_<Statement_t> Parser::ioOp() {
    IoOp_t temp = std::make_shared<IoOp>(*currentToken);
    consumeToken();
    return temp;
}

_<Statement_t> Parser::ifElse() {
    consumeToken();  // consume "if"
    Block_t thenBranch = std::make_shared<Block>();
    while ((*currentToken).type != ELSE && (*currentToken).type != END && currentToken) {
        auto result = statement();
        if (_check(result))
            return ErrorHandler::addContext(result, "@ 'if' branch: " + (*currentToken).toString());
        thenBranch->statements.push_back(std::get<Statement_t>(result));
    }
    Block_t elseBranch = nullptr;
    if ((*currentToken).type == ELSE) {
        consumeToken();  // consume "else"
        elseBranch = std::make_shared<Block>();
        while ((*currentToken).type != END && currentToken) {
            auto result = statement();
            if (_check(result))
                return ErrorHandler::addContext(result, "@ 'else' branch: " + (*currentToken).toString());
            elseBranch->statements.push_back(std::get<Statement_t>(result));
        }
    }
    if ((*currentToken).type == END) {
        consumeToken();  // consume "end"
    } else {
        return ErrorHandler::createError(ErrorCode::MISSING_END_TOKEN, "Expected 'end' after if-else statement: " + (*currentToken).toString());
    }
    return std::make_shared<IfElse>(thenBranch, elseBranch);
}

_<Statement_t> Parser::whileStatement() {
    consumeToken();  // consume "while"
    Block_t body = std::make_shared<Block>();
    while ((*currentToken).type != END && currentToken) {
        auto result = statement();
        if (_check(result))
            return ErrorHandler::addContext(result, "@ 'while' body: " + (*currentToken).chordLexeme);
        body->statements.push_back(std::get<Statement_t>(result));
    }
    if ((*currentToken).type == END) {
            consumeToken();  // consume "end"
    } else {    
        //error stop building syntax tree
        return ErrorHandler::createError(ErrorCode::MISSING_END_TOKEN, "Expected 'end' after while statement: " + (*currentToken).toString());
    }
    return std::make_shared<While>(body);
}

_<Statement_t> Parser::identifierCall() {
    consumeToken();  // consume "f"
    auto id = literal();
    if (_check(id))
        return ErrorHandler::addContext(id, "@ Identifier call: " + (*currentToken).toString());

    if ((*currentToken).type == END) {
        consumeToken();  // consume "end"
    } else {
        //error stop building syntax tree
        return ErrorHandler::createError(ErrorCode::MISSING_END_TOKEN, "Expected 'end' after identifier call: " + (*currentToken).toString());
    }
    return std::make_shared<IdentifierCall>(std::dynamic_pointer_cast<Literal>(std::get<Statement_t>(id)));
}

_<Statement_t> Parser::variableDeclaration() {
    consumeToken();  // consume "var"
    auto id = literal();
    if (_check(id))
        return ErrorHandler::addContext(id, "@ Variable declaration: " + (*currentToken).toString());

    if ((*currentToken).type == END) {
        consumeToken();  // consume "end"
    } else {
        //error stop building syntax tree
        return ErrorHandler::createError(ErrorCode::MISSING_END_TOKEN, "Expected 'end' after variable declaration: " + (*currentToken).toString());
    }
    return std::make_shared<VariableDeclaration>(std::dynamic_pointer_cast<Literal>(std::get<Statement_t>(id)));
}

_<Statement_t> Parser::variableOp() {
    VariableOp_t temp = std::make_shared<VariableOp>(*currentToken);
    consumeToken();
    return temp;
}

_<Statement_t> Parser::definition() {
    consumeToken();  // consume "def"
    auto id = literal();
    if (_check(id))
        return ErrorHandler::addContext(id, "@ Definition: " + (*currentToken).toString());

    if ((*currentToken).type == END) {
        consumeToken();  // consume "end"
    } else {
        //error stop building syntax tree
        return ErrorHandler::createError(ErrorCode::MISSING_END_TOKEN, "Expected 'end' after definition identifier: " + (*currentToken).toString());
    }

    Block_t body = std::make_shared<Block>();
    while ((*currentToken).type != END && currentToken) {
        auto result = statement();
        if (_check(result))
            return ErrorHandler::addContext(result, "@ Definition body: " + (*currentToken).toString());
        body->statements.push_back(std::get<Statement_t>(result));
    }
    if ((*currentToken).type == END) {
        consumeToken();  // consume "end"
    } else {
        //error stop building syntax tree
        return ErrorHandler::createError(ErrorCode::MISSING_END_TOKEN, "Expected 'end' after definition: " + (*currentToken).toString());
    }
    return std::make_shared<Definition>(std::dynamic_pointer_cast<Literal>(std::get<Statement_t>(id)), body);
}




