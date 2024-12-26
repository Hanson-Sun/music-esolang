#pragma once
#include <string>

#include "ASTNodes.h"
#include "Tokenizer.h"

// ok it does something idk what tho
class Parser {
 public:
    Program_t program;

    explicit Parser(Tokenizer& tokenizer)
        : program(std::make_shared<Program>()),
          tokenizer(tokenizer),
          currentToken(tokenizer.begin()) {}
    _<Statement_t> parse();

    // for the interpreter
    class ParserIterator {
     public:
        _<Statement_t> currentStatement;
        int delay = 0;
        using value_type = _<Statement_t>;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;
        using iterator_category = std::forward_iterator_tag;

        explicit ParserIterator(Parser* parser)
            : currentStatement(std::make_shared<NoOp>()), parser(parser) {
            currentStatement = parser->parse();
        }
        reference operator*() { return currentStatement; }
        ParserIterator& operator++() {
            if (parser->currentToken) currentStatement = parser->parse();
            return *this;
        }
        explicit operator bool() {
            if (!static_cast<bool>(parser->currentToken)) {
                return delay++ < 1;
            }
            return static_cast<bool>(parser->currentToken);
        }

     private:
        Parser* parser;
    };

    ParserIterator begin() { return ParserIterator(this); }
    ParserIterator end() { return ParserIterator(nullptr); }

 private:
    // TODO change this to _<Token>
    Token consumeToken();
    void expectToken(TokenType receivedType, const std::string& errorMessage);

    _<Statement_t> statement();
    _<Statement_t> literal();
    _<Statement_t> identifier();
    _<Statement_t> identifierCall();
    _<Statement_t> arithmeticOp();
    _<Statement_t> logicalOp();
    _<Statement_t> stackOp();
    _<Statement_t> ioOp();
    _<Statement_t> ifElse();
    _<Statement_t> whileStatement();
    _<Statement_t> variableOp();
    _<Statement_t> variableDeclaration();
    _<Statement_t> definition();

 private:
    Tokenizer& tokenizer;
    Tokenizer::TokenizerIterator currentToken;
};
