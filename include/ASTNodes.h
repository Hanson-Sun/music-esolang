#pragma once
#include "ASTVisitor.h"
#include "Tokenizer.h"

#include <memory>
#include <vector>

using Statement_t = std::shared_ptr<Statement>;
using Block_t = std::shared_ptr<Block>;
using Program_t = std::shared_ptr<Program>;
using Definition_t = std::shared_ptr<Definition>;
using IdentifierCall_t = std::shared_ptr<IdentifierCall>;
using Literal_t = std::shared_ptr<Literal>;
using ArithmeticOp_t = std::shared_ptr<ArithmeticOp>;
using LogicalOp_t = std::shared_ptr<LogicalOp>;
using StackOp_t = std::shared_ptr<StackOp>;
using IoOp_t = std::shared_ptr<IoOp>;
using ControlFlow_t = std::shared_ptr<ControlFlow>;
using IfElse_t = std::shared_ptr<IfElse>;
using While_t = std::shared_ptr<While>;
using VariableOp_t = std::shared_ptr<VariableOp>;
using VariableDeclaration_t = std::shared_ptr<VariableDeclaration>;
using Comment_t = std::shared_ptr<Comment>;

struct ASTNode {
    virtual ~ASTNode() = default;
    virtual void accept(ASTVisitor& visitor) = 0; // cannot be const unfortunately
};

// --- Statement Nodes ---
// <statement> ::= <literal>
//               | <arithmetic-op>
//               | <logical-op>
//               | <stack-op>
//               | <io-op>
//               | <control-flow>
//               | <variable-op>
//               | <definition>
//               | <comment>
//               | <identifier-call>
struct Statement : ASTNode {
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <program> ::= { <statement> }
struct Program : ASTNode {
    std::vector<Statement_t> statements;
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <literal> ::= <base 12 number> { "end" }
struct Literal : Statement {
    Token token;
    explicit Literal(Token token) : token(token) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <arithmetic-op> ::= "+" | "-" | "*" | "/" | "%"
struct ArithmeticOp : Statement {
    Token op;
    Statement_t left;
    Statement_t right;
    explicit ArithmeticOp(Token op) : op(op) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <logical-op> ::= "=" | "<" | ">" | "&" | "|" | "~"
struct LogicalOp : Statement {
    Token op;
    Statement_t child;
    explicit LogicalOp(Token t) : op(t) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <stack-op> ::= "pop" | "dup" | "dup." | "swap" | "size"
struct StackOp : Statement {
    Token op;
    explicit StackOp(Token t) : op(t) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <io-op> ::= "input" | "print" | "print-" | "debug"
struct IoOp : Statement {
    Token op;
    explicit IoOp(Token t) : op(t) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <control-flow> ::= <if-else> | <while>
struct ControlFlow : Statement {
    ControlFlow() {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <block> ::= { <statement> }
struct Block : Statement {
    std::vector<Statement_t> statements;
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <if-else> ::= "if" <block> [ "else" <block> ] "end"
struct IfElse : ControlFlow {
    Block_t then_branch;
    Block_t else_branch;
    IfElse(Block_t then_branch, Block_t else_branch)
        : then_branch(then_branch), else_branch(else_branch) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <while> ::= "while" <block> "end"
struct While : ControlFlow {
    Block_t body;
    explicit While(Block_t body) : body(body) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <identifier-call> ::= "f" <identifier> "end"
struct IdentifierCall : Statement {
    Literal_t identifier;
    explicit IdentifierCall(Literal_t id) : identifier(id) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <variable-op> ::= "!"       (* Store: top of stack is value, second is address *)
//                  | "@"       (* Load: push value at address to the stack *)
//                  | "^"       (* Free: free memory at the address on top of the stack *)
struct VariableOp : Statement {
    Token op;
    explicit VariableOp(Token t) : op(t) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <variable-dec> ::= "var" <identifier>
struct VariableDeclaration : Statement {
    Literal_t identifier;
    explicit VariableDeclaration(Literal_t identifier) : identifier(identifier) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <definition> ::= "def" <identifier> <block> "end"
struct Definition : Statement {
    Literal_t identifier;
    Block_t body;
    explicit Definition(Literal_t id, Block_t body) : identifier(id), body(body) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <comment> ::= "#" { <note> } "#"
struct Comment : Statement {
    Token token;
    explicit Comment(Token t) : token(t) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};
