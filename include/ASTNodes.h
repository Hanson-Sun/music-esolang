#pragma once
#include "ASTVisitor.h"
#include "Tokenizer.h"

struct ASTNode {
    virtual ~ASTNode() = default;
    virtual void accept(ASTVisitor& visitor) = 0;
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
    std::vector<Statement*> statements;
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <literal> ::= <base 12 number> { "end" }
struct Literal : Statement {
    Token token;
    Literal(Token token) : token(token) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <arithmetic-op> ::= "+" | "-" | "*" | "/" | "%"
struct ArithmeticOp : Statement {
    Token op;
    ArithmeticOp(Token op) : op(op) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <logical-op> ::= "=" | "<" | ">" | "&" | "|" | "~"
struct LogicalOp : Statement {
    Token op;
    LogicalOp(Token t) : op(t) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <stack-op> ::= "pop" | "dup" | "dup." | "swap" | "size"
struct StackOp : Statement {
    Token op;
    StackOp(Token t) : op(t) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <io-op> ::= "input" | "print" | "print-" | "debug"
struct IoOp : Statement {
    Token op;
    IoOp(Token t) : op(t) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <control-flow> ::= <if-else> | <while>
struct ControlFlow : Statement {
    ControlFlow() {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <block> ::= { <statement> }
struct Block : Statement {
    std::vector<Statement*> statements;
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <if-else> ::= "if" <block> [ "else" <block> ] "end"
struct IfElse : ControlFlow {
    Block* then_branch;
    Block* else_branch;
    IfElse(Block* then_branch, Block* else_branch)
        : then_branch(then_branch), else_branch(else_branch) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <while> ::= "while" <block> "end"
struct While : ControlFlow {
    Block* body;
    While(Block* body) : body(body) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <identifier> ::= <pitch> | <chord> { <pitch> | <chord> }
struct Identifier : Statement {
    Token token;  // Contains the identifier name
    Identifier(Token t) : token(t) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <identifier-call> ::= "f" <identifier> "end"
struct IdentifierCall : Statement {
    Identifier* identifier;
    IdentifierCall(Identifier* id) : identifier(id) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <variable-op> ::= "var" <identifier>
//                  | "!"       (* Store: top of stack is value, second is address *)
//                  | "@"       (* Load: push value at address to the stack *)
//                  | "^"       (* Free: free memory at the address on top of the stack *)
struct VariableOp : Statement {
    Token op;
    VariableOp(Token t) : op(t) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <variable-op> ::= "var" <identifier>
struct VariableDeclaration : Statement {
    Identifier identifier;
    VariableDeclaration(Token identifier) : identifier(identifier) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <definition> ::= "def" <identifier> <block> "end"
struct Definition : Statement {
    Identifier* identifier;
    Block* body;
    Definition(Identifier* id, Block* body) : identifier(id), body(body) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

// <comment> ::= "#" { <note> } "#"
struct Comment : Statement {
    Token token;
    Comment(Token t) : token(t) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};
