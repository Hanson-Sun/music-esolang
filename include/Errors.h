#pragma once
#include <variant>
#include <sstream>
#include <string>
#include <unordered_map>
#include <iostream>


#define ERR_RESET   "\033[0m"
#define ERR_BLACK   "\033[30m"      
#define ERR_RED     "\033[31m"      
#define ERR_GREEN   "\033[32m"      
#define ERR_YELLOW  "\033[33m"      
#define ERR_BLUE    "\033[34m"      
#define ERR_MAGENTA "\033[35m"      
#define ERR_CYAN    "\033[36m"      
#define ERR_WHITE   "\033[37m"      

enum ErrorCode : int8_t{
    NONE,
    INVALID_TOKEN,
    STACK_UNDERFLOW,
    INVALID_ADDRESS,
    IDENTIFIER_NOT_FOUND,
    IDENTIFIER_ALREADY_DEFINED,
    MEMORY_ALLOCATION_FAILED,
    INVALID_STATEMENT,
    MISSING_END_TOKEN,
    INVALID_VALUE
};

const std::unordered_map<ErrorCode, std::string> ErrorMessages = {
    {ErrorCode::NONE, "No error"},
    {ErrorCode::INVALID_TOKEN, "Invalid Token"},
    {ErrorCode::STACK_UNDERFLOW, "Stack underflow"},
    {ErrorCode::INVALID_ADDRESS, "Invalid address"},
    {ErrorCode::IDENTIFIER_NOT_FOUND, "Identifier not found"},
    {ErrorCode::IDENTIFIER_ALREADY_DEFINED, "Identifier already defined"},
    {ErrorCode::MEMORY_ALLOCATION_FAILED, "Memory allocation failed"},
    {ErrorCode::INVALID_STATEMENT, "Invalid statement"},
    {ErrorCode::MISSING_END_TOKEN, "Missing end token"},
    {ErrorCode::INVALID_VALUE, "Invalid value"}
};

struct Error {
    ErrorCode code;
    std::string message;

    Error(ErrorCode code, std::string message) : code(code), message(std::move(message)) {}
    Error(ErrorCode code) : code(code), message() {}
    Error() : code(ErrorCode::NONE), message() {}
};


template<typename T>
class Result {
public:
    Result(T value) : is_error(false), value(value) {}
    Result(Error error) : is_error(true), error(std::move(error)) {}

    bool has_value() const { return !is_error; }
    bool has_error() const { return is_error; }

    T& get_value() { return value; }
    const T& get_value() const { return value; }

    Error& get_error() { return error; }
    const Error& get_error() const { return error; }

private:
    bool is_error;
    union {
        T value;
        Error error;
    };
};

template<>
class Result<void> {
public:
    Result() : is_error(false) {}
    Result(Error error) : is_error(true), error(std::move(error)) {}

    bool has_value() const { return !is_error; }
    bool has_error() const { return is_error; }

    void get_value() const {}
    Error& get_error() { return error; }
    const Error& get_error() const { return error; }

private:
    bool is_error;
    Error error;
};

template<typename T = std::monostate>
using _ = std::variant<T, Error>;

template<typename T>
inline bool _check(const _<T>& result) {
    return std::holds_alternative<Error>(result);
}

class ErrorHandler {
public:
    inline static void printError(const Error& error) {
        std::cerr << ERR_RED << "Error: " << ErrorMessages.at(error.code) << ". " << ERR_MAGENTA << error.message << ERR_RESET << std::endl;
    }

    inline static Error createError(ErrorCode code, const std::string& context = "", const std::string& message = "") {
        std::string m = message;
        if (!context.empty()) {
            m += "\n  ~~~>[" + context + "]";
        }
        return Error(code, m);
    }

    template<typename T>
    inline static Error addContext(std::variant<T, Error> error, const std::string& context) {
        std::get<Error>(error).message += "\n  ~~~>[" + context + "]";
        return std::get<Error>(error);
    }
};


