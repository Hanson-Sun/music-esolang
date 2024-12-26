#include "Interpreter.h"

#include <iostream>

_<> Interpreter::interpret(Statement_t statement) { 
    return statement->accept(*this);
}

_<> Interpreter::interpret(Program_t program) { 
    return program->accept(*this);
}

_<> Interpreter::visit(const Program& node) {
    for (const auto& statement : node.statements) {
        if (auto result = statement->accept(*this); _check(result)) {
            return ErrorHandler::addContext(result, "@ Main Program");
        }
    }
    return std::monostate();
}
_<> Interpreter::visit(const Literal& node) { 
    if (auto result = push(node.token.value); _check(result)) 
        return ErrorHandler::addContext(result, "@ Literal: " + node.token.toString());
    return std::monostate();
}

_<> Interpreter::visit(const ArithmeticOp& node) {
    switch (node.op.type) {
        case TokenType::ADD: {
            auto a = pop();
            if (_check(a))
                return ErrorHandler::addContext(a, "@ Arithmetic + operation: " + node.op.toString());

            auto b = pop();
            if (_check(b))
                return ErrorHandler::addContext(b, "@ Arithmetic + operation: " + node.op.toString());

            if (auto result = push(std::get<int64_t>(a) + std::get<int64_t>(b)); _check(result))
                return ErrorHandler::addContext(result, "@ Arithmetic + operation: " + node.op.toString());
            break;
        }
        case TokenType::SUB: {
            auto a = pop();
            if (_check(a))
                return ErrorHandler::addContext(a, "@ Arithmetic - operation: " + node.op.toString());

            auto b = pop();
            if (_check(b))
                return ErrorHandler::addContext(b, "@ Arithmetic - operation: " + node.op.toString());

            if (auto result = push(std::get<int64_t>(a) - std::get<int64_t>(b)); _check(result))
                return ErrorHandler::addContext(result, "@ Arithmetic - operation: " + node.op.toString());
            break;
        }
        case TokenType::MUL: {
            auto a = pop();
            if (_check(a))
                return ErrorHandler::addContext(a, "@ Arithmetic * operation: " + node.op.toString());

            auto b = pop();
            if (_check(b))
                return ErrorHandler::addContext(b, "@ Arithmetic * operation: " + node.op.toString());

            if (auto result = push(std::get<int64_t>(a) * std::get<int64_t>(b)); _check(result))
                return ErrorHandler::addContext(result, "@ Arithmetic * operation: " + node.op.toString());
            break;
        }
        case TokenType::DIV: {
            auto a = pop();
            if (_check(a))
                return ErrorHandler::addContext(a, "@ Arithmetic / operation: " + node.op.toString());

            auto b = pop();
            if (_check(b))
                return ErrorHandler::addContext(b, "@ Arithmetic / operation: " + node.op.toString());

            if (auto result = push(std::get<int64_t>(a) / std::get<int64_t>(b)); _check(result))
                return ErrorHandler::addContext(result, "@ Arithmetic / operation: " + node.op.toString());
            break;
        }
        case TokenType::MOD: {
            auto a = pop();
            if (_check(a))
                return ErrorHandler::addContext(a, "@ Arithmetic % operation: " + node.op.toString());

            auto b = pop();
            if (_check(b))
                return ErrorHandler::addContext(b, "@ Arithmetic % operation: " + node.op.toString());

            if (auto result = push(std::get<int64_t>(a) % std::get<int64_t>(b)); _check(result))
                return ErrorHandler::addContext(result, "@ Arithmetic % operation: " + node.op.toString());
            break;
        }
        default:
            return ErrorHandler::createError(ErrorCode::INVALID_TOKEN, "@ Arithmetic operation: " + node.op.toString());
            break;
    };
    return std::monostate();
}

_<> Interpreter::visit(const LogicalOp& node) {
    switch (node.op.type) {
        case TokenType::EQ: {
            auto a = pop();
            if (_check(a))
                return ErrorHandler::addContext(a, "@ Logical == operation: " + node.op.toString());

            auto b = pop();
            if (_check(b))
                return ErrorHandler::addContext(b, "@ Logical == operation: " + node.op.toString());

            if (auto result = push(std::get<int64_t>(a) == std::get<int64_t>(b)); _check(result))
                return ErrorHandler::addContext(result, "@ Logical == operation: " + node.op.toString());
            break;
        }
        case TokenType::LESS: {
            auto a = pop();
            if (_check(a))
                return ErrorHandler::addContext(a, "@ Logical < operation: " + node.op.toString());

            auto b = pop();
            if (_check(b))
                return ErrorHandler::addContext(b, "@ Logical < operation: " + node.op.toString());

            if (auto result = push(std::get<int64_t>(a) < std::get<int64_t>(b)); _check(result))
                return ErrorHandler::addContext(result, "@ Logical < operation: " + node.op.toString());
            break;
        }
        case TokenType::GREATER: {
            auto a = pop();
            if (_check(a))
                return ErrorHandler::addContext(a, "@ Logical > operation: " + node.op.toString());

            auto b = pop();
            if (_check(b))
                return ErrorHandler::addContext(b, "@ Logical > operation: " + node.op.toString());

            if (auto result = push(std::get<int64_t>(a) > std::get<int64_t>(b)); _check(result))
                return ErrorHandler::addContext(result, "@ Logical > operation: " + node.op.toString());
            break;
        }
        case TokenType::AND: {
            auto a = pop();
            if (_check(a))
                return ErrorHandler::addContext(a, "@ Bitwise & operation: " + node.op.toString());

            auto b = pop();
            if (_check(b))
                return ErrorHandler::addContext(b, "@ Bitwise & operation: " + node.op.toString());

            if (auto result = push(std::get<int64_t>(a) & std::get<int64_t>(b)); _check(result))
                return ErrorHandler::addContext(result, "@ Bitwise & operation: " + node.op.toString());
            break;
        }
        case TokenType::OR: {
            auto a = pop();
            if (_check(a))
                return ErrorHandler::addContext(a, "@ Bitwise | operation: " + node.op.toString());

            auto b = pop();
            if (_check(b))
                return ErrorHandler::addContext(b, "@ Bitwise | operation: " + node.op.toString());

            if (auto result = push(std::get<int64_t>(a) | std::get<int64_t>(b)); _check(result))
                return ErrorHandler::addContext(result, "@ Bitwise | operation: " + node.op.toString());
            break;
        }
        case TokenType::NOT: {
            auto a = pop();
            if (_check(a))
                return ErrorHandler::addContext(a, "@ Bitwise ~ operation: " + node.op.toString());

            if (auto result = push(~std::get<int64_t>(a)); _check(result))
                return ErrorHandler::addContext(result, "@ Bitwise ~ operation: " + node.op.toString());
            break;
        }
        default:
            return ErrorHandler::createError(ErrorCode::INVALID_TOKEN, "@ Logical operation: " + node.op.toString());
            break;
    };
    return std::monostate();
}

_<> Interpreter::visit(const StackOp& node) {
    switch (node.op.type) {
        case TokenType::POP: {
            if (auto result = pop(); _check(result))
                return ErrorHandler::addContext(result, "@ Stack pop operation: " + node.op.toString());
            break;
        }
        case TokenType::DUP: {
            auto value = top();
            if (_check(value))
                return ErrorHandler::addContext(value, "@ Stack dup operation: " + node.op.toString());

            if (auto result = push(std::get<int64_t>(value)); _check(result))
                return ErrorHandler::addContext(result, "@ Stack dup operation: " + node.op.toString());
            break;
        }
        case TokenType::DUPDOT: {
            auto t = top();
            if (_check(t))
                return ErrorHandler::addContext(t, "@ Stack dup. operation: " + node.op.toString());

            if (auto result = pop(); _check(result))
                return ErrorHandler::addContext(result, "@ Stack dup. operation: " + node.op.toString());

            auto s = top();
            if (_check(s))
                return ErrorHandler::addContext(s, "@ Stack dup. operation: " + node.op.toString());

            if (auto result = push(std::get<int64_t>(t)); _check(result))
                return ErrorHandler::addContext(result, "@ Stack dup. operation: " + node.op.toString());

            if (auto result = push(std::get<int64_t>(s)); _check(result))
                return ErrorHandler::addContext(result, "@ Stack dup. operation: " + node.op.toString());
            break;
        }
        case TokenType::SWAP: {
            auto a = pop();
            if (_check(a))
                return ErrorHandler::addContext(a, "@ Stack swap operation: " + node.op.toString());

            auto b = pop();
            if (_check(b))
                return ErrorHandler::addContext(b, "@ Stack swap operation: " + node.op.toString());

            if (auto result = push(std::get<int64_t>(a)); _check(result))
                return ErrorHandler::addContext(result, "@ Stack swap operation: " + node.op.toString());

            if (auto result = push(std::get<int64_t>(b)); _check(result))
                return ErrorHandler::addContext(result, "@ Stack swap operation: " + node.op.toString());
            break;
        }
        case TokenType::SIZE: {
            auto size = stack.size();
            if (auto result = push(size); _check(result))
                return ErrorHandler::addContext(result, "@ Stack size operation");
            break;
        }
        default:
            return ErrorHandler::createError(ErrorCode::INVALID_TOKEN, "@ Stack operation: " + node.op.toString());
            break;
    };
    return  std::monostate();
}

_<> Interpreter::visit(const IoOp& node) {
    switch (node.op.type) {
        case TokenType::DEBUG: {
            for (auto it = stack.rbegin(); it != stack.rend(); ++it) {
                std::cout << *it << " ";
            }
            std::cout << std::endl;
        }
        case TokenType::PRINT: {
            auto value = pop();
            if (_check(value))
                return ErrorHandler::addContext(value, "@ IO print operation: " + node.op.toString());

            std::cout << std::get<int64_t>(value) << std::endl;
            break;
        }
        case TokenType::PRINTCHAR: {
            auto value = pop();
            if (_check(value))
                return ErrorHandler::addContext(value, "@ IO printchar operation: " + node.op.toString());

            std::cout << static_cast<char>(std::get<int64_t>(value)) << std::endl;
            break;
        }
        case TokenType::INPUT: {
            int64_t value;
            std::cin >> value;
            if (auto result = push(value); _check(result))
                return ErrorHandler::addContext(result, "@ IO input operation: " + node.op.toString());
            break;
        }
        default:
            return ErrorHandler::createError(ErrorCode::INVALID_TOKEN, "@ IO operation: " + node.op.toString());
            break;
    }
    return std::monostate();
}

_<> Interpreter::visit(const IfElse& node) {
    //then branch
    auto value = pop();
    if (_check(value))
        return ErrorHandler::addContext(value, "@ IfElse operation: " + node.op.toString());
    if (std::get<int64_t>(value) != 0) {
        if (auto result = node.then_branch->accept(*this); _check(result))
            return ErrorHandler::addContext(result, "@ IfElse operation: " + node.op.toString());
    } else {
        // else branch
        if (node.else_branch) {
            if (auto result = node.else_branch->accept(*this); _check(result))
                return ErrorHandler::addContext(result, "@ IfElse operation: " + node.op.toString());
        }
    }
    return std::monostate();
}

_<> Interpreter::visit(const While& node) {
    while (true) {
        auto value = pop();
        if (_check(value))
            return ErrorHandler::addContext(value, "@ While operation: " + node.op.toString());
        if (std::get<int64_t>(value) == 0) {
            break;
        }
        if (auto result = node.body->accept(*this); _check(result))
            return ErrorHandler::addContext(result, "@ While operation: " + node.op.toString());
    }
    return std::monostate();
}

_<> Interpreter::visit(const Block& node) {
    if (auto result = enterScope(); _check(result)) 
        return ErrorHandler::addContext(result, "@ Block enter scope");

    for (const auto& statement : node.statements) {
        if (auto result = statement->accept(*this); _check(result)) {
            return ErrorHandler::addContext(result, "@ Block");
        }
    }

    if (auto result = exitScope(); _check(result)) 
        return ErrorHandler::addContext(result, "@ Block exit scope");
    
    return std::monostate();
}

_<> Interpreter::visit(const VariableOp& node) {
    switch (node.op.type) {
        case TokenType::STORE: {
            auto value = pop();
            if (_check(value)) 
                return ErrorHandler::addContext(value, "@ Variable store operation: " + node.op.toString());

            auto address = pop();
            if (_check(address)) 
                return ErrorHandler::addContext(address, "@ Variable store operation: " + node.op.toString());

            if (auto result = a_set(std::get<int64_t>(value), std::get<int64_t>(address)); _check(result)) 
                return ErrorHandler::addContext(result, "@ Variable store operation: " + node.op.toString());

            break;
        }
        case TokenType::LOAD: {
            auto address = pop();
            if (_check(address)) 
                return ErrorHandler::addContext(address, "@ Variable load operation: " + node.op.toString());

            auto value = a_at(std::get<int64_t>(address));
            if (_check(value)) 
                return ErrorHandler::addContext(value, "@ Variable load operation: " + node.op.toString());

            if (auto result = push(std::get<int64_t>(value)); _check(result)) 
                return ErrorHandler::addContext(result, "@ Variable load operation: " + node.op.toString());

            break;
        }
        case TokenType::FREE: {
            auto address = pop();
            if (_check(address))
                return ErrorHandler::createError(ErrorCode::STACK_UNDERFLOW, "@ Variable free operation: " + node.op.toString());
            

            if (auto result = a_free(std::get<int64_t>(address)); _check(result))
                return ErrorHandler::addContext(result, "@ Variable free operation: " + node.op.toString());

            break;
        }
        default:
            // really shouldnt happen
            return ErrorHandler::createError(ErrorCode::INVALID_TOKEN, "@ Variable operation: " + node.op.toString());
            break;
    };
    return std::monostate();
}

_<> Interpreter::visit(const VariableDeclaration& node) {
    auto result = setVar(node.identifier->token.value, node.value->token.value);
    if (_check(result)) {
        return ErrorHandler::addContext(result, "@ Variable declaration: " + node.identifier->token.toString());
    }
    return std::monostate();
}

_<> Interpreter::visit(const Definition& node) {
    auto result = setFunc(node.identifier->token.value, node.body);
    if (_check(result)) 
        return ErrorHandler::addContext(result, "@ Function definition: " + node.identifier->token.toString());
    return std::monostate();
}

_<> Interpreter::visit(const IdentifierCall& node) {
    auto funcResult = getFunc(node.identifier->token.value);
    auto varResult = getVar(node.identifier->token.value);
    if (_check(funcResult) && _check(varResult))
        return ErrorHandler::addContext(funcResult, "@ Identifier call: " + node.identifier->token.toString());
    return std::monostate();
}

_<> Interpreter::visit(const NoOp&) {
    return std::monostate();
}

_<int64_t> Interpreter::pop() {
    if (stack.empty()) {
        return ErrorHandler::createError(ErrorCode::STACK_UNDERFLOW, "@ Popping from stack");
    }

    int64_t value = stack.back();
    stack.pop_back();
    return value;
}

_<> Interpreter::push(int64_t value) { stack.push_back(value); return std::monostate(); }

_<int64_t> Interpreter::top() { return stack.back();}

_<> Interpreter::enterScope() { scopes.push_back(Scope()); return std::monostate();}

_<> Interpreter::exitScope() { 
    if (scopes.empty()) {
        return ErrorHandler::createError(ErrorCode::STACK_UNDERFLOW, "@ Exiting scope");
    }

    scopes.pop_back(); 
    return std::monostate();
}

_<> Interpreter::getVar(int64_t varName) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto var_it = it->var_defs.find(varName);
        if (var_it != it->var_defs.end()) {
            push(var_it->second);
            return std::monostate();
        }
    }
    return ErrorHandler::createError(ErrorCode::IDENTIFIER_NOT_FOUND,
                                     "@ Getting variable: " + std::to_string(varName));
}

_<> Interpreter::setVar(int64_t varName, int64_t value) {
    // check if variable is already defined
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto var_it = it->var_defs.find(varName);
        if (var_it != it->var_defs.end()) {
            return ErrorHandler::createError(ErrorCode::IDENTIFIER_ALREADY_DEFINED,
                                             "@ Setting variable: " + std::to_string(varName));
        }
    }

    scopes.back().var_defs[varName] = value;
    return std::monostate();
}

_<> Interpreter::getFunc(int64_t funcName) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto func_it = it->func_defs.find(funcName);
        if (func_it != it->func_defs.end()) {
            // execute function
            if (auto result = func_it->second->accept(*this); _check(result)) {
                return ErrorHandler::addContext(result, "@ Function: " + std::to_string(funcName));
            }
            
            return std::monostate();
        }
    }
    return ErrorHandler::createError(ErrorCode::IDENTIFIER_NOT_FOUND, "@ Getting function: " + std::to_string(funcName));
}

_<> Interpreter::setFunc(int64_t funcName, const Block_t func) {
    // check if function is already defined
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto func_it = it->func_defs.find(funcName);
        if (func_it != it->func_defs.end()) {
            return ErrorHandler::createError(ErrorCode::IDENTIFIER_ALREADY_DEFINED, "@ Setting function: " + std::to_string(funcName));
        }
    }

    scopes.back().func_defs[funcName] = func;
    return std::monostate();
}

_<int64_t> Interpreter::a_malloc() {
    if (!arena_free.empty()) {
        int64_t address = arena_free.front();
        arena_free.pop();
        arena_used[address] = true;
        return address;
    }

    arena.push_back(0);
    arena_used.push_back(true);
    return (int64_t) (arena.size() - 1);
}

_<> Interpreter::a_free(int64_t address) {
    if (address < 0 || address >= static_cast<int64_t>(arena.size())) {
        return ErrorHandler::createError(
            ErrorCode::INVALID_ADDRESS,
            "@ freeing variable at address: " + std::to_string(address));
    }

    if (arena_used[address]) {
        arena_free.push(address);
        arena_used[address] = false;
    }

    return std::monostate();
}

_<int64_t> Interpreter::a_at(int64_t address) {
    if (address < 0 || address >= static_cast<int64_t>(arena.size()) || !arena_used[address]) {
        return ErrorHandler::createError(ErrorCode::INVALID_ADDRESS, "@ getting variable value at address: " + std::to_string(address));
    }
    return arena[address];
}

_<> Interpreter::a_set(int64_t address, int64_t value) {
    if (address < 0 || address >= static_cast<int64_t>(arena.size()) || !arena_used[address]) {
        return ErrorHandler::createError(ErrorCode::INVALID_ADDRESS, "@ setting variable value at address: " + std::to_string(address));
    }
    arena[address] = value;
    return std::monostate();
}


