#pragma once
#include <variant>
#include <string>
#include <unordered_map>
#include <iostream>

enum ErrorCode : int8_t{
    NONE,
    INVALID_TOKEN,
    STACK_UNDERFLOW,
    INVALID_ADDRESS,
    VARIABLE_NOT_FOUND,
    VARIABLE_ALREADY_DEFINED,
    FUNCTION_NOT_FOUND,
    FUNCTION_ALREADY_DEFINED,
    MEMORY_ALLOCATION_FAILED,
};

const std::unordered_map<ErrorCode, std::string> ErrorMessages = {
    {ErrorCode::NONE, "No error"},
    {ErrorCode::STACK_UNDERFLOW, "Stack underflow"},
    {ErrorCode::INVALID_ADDRESS, "Invalid address"},
    {ErrorCode::VARIABLE_NOT_FOUND, "Variable not found"},
    {ErrorCode::VARIABLE_ALREADY_DEFINED, "Variable already defined"},
    {ErrorCode::FUNCTION_NOT_FOUND, "Function not found"},
    {ErrorCode::FUNCTION_ALREADY_DEFINED, "Function already defined"},
    {ErrorCode::MEMORY_ALLOCATION_FAILED, "Memory allocation failed"}
};

struct Error {
    ErrorCode code;
    std::string message;
    Error(ErrorCode code, const std::string& message) : code(code), message(message) {}
    Error(ErrorCode code) : code(code), message("") {}
    Error() : code(ErrorCode::NONE), message("") {}
};


template<typename T = std::monostate>
using _ = std::variant<T, Error>;

template<typename T>
bool _check(const _<T>& result) {
    return std::holds_alternative<Error>(result);
}

class ErrorHandler {
public:
    static void printError(const Error& error) {
        std::cerr << "Error: " << ErrorMessages.at(error.code) << ". " << error.message << std::endl;
    }

    static Error createError(ErrorCode code, const std::string& context = "") {
        std::string message = ErrorMessages.at(code);
        if (!context.empty()) {
            message += "\n[" + context + "]";
        }
        return Error(code, message);
    }

    template<typename T>
    static Error addContext(std::variant<T, Error> error, const std::string& context) {
        std::cout << context << std::endl;
        std::get<Error>(error).message += "\n[" + context + "]";
        return std::get<Error>(error);
    }
};


