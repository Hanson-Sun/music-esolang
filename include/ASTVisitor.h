#pragma once

#include "Errors.h"

struct Block;
struct Program;
struct Definition;
struct IdentifierCall;
struct Literal;
struct ArithmeticOp;
struct LogicalOp;
struct StackOp;
struct IoOp;
struct IfElse;
struct While;
struct VariableOp;
struct VariableDeclaration;


// TODO: refactor these to return an error value instead of throwing an exception

class ASTVisitor {
 public:
    virtual ~ASTVisitor() = default;
    virtual _<> visit(const Program& node) = 0;
    virtual _<> visit(const Literal& node) = 0;
    virtual _<> visit(const ArithmeticOp& node) = 0;
    virtual _<> visit(const LogicalOp& node) = 0;
    virtual _<> visit(const StackOp& node) = 0;
    virtual _<> visit(const IoOp& node) = 0;
    virtual _<> visit(const Block& node) = 0;
    virtual _<> visit(const IfElse& node) = 0;
    virtual _<> visit(const While& node) = 0;
    virtual _<> visit(const IdentifierCall& node) = 0;
    virtual _<> visit(const VariableOp& node) = 0;
    virtual _<> visit(const VariableDeclaration& node) = 0;
    virtual _<> visit(const Definition& node) = 0;
};

class ASTPrinter : ASTVisitor {
    ASTPrinter() = default;
    void visit(const Statement& node) override;
    void visit(const Program& node) override;
    void visit(const Literal& node) override;
    void visit(const ArithmeticOp& node) override;
    void visit(const LogicalOp& node) override;
    void visit(const StackOp& node) override;
    void visit(const IoOp& node) override;
    void visit(const ControlFlow& node) override;
    void visit(const Block& node) override;
    void visit(const IfElse& node) override;
    void visit(const While& node) override;
    void visit(const Literal& node) override;
    void visit(const IdentifierCall& node) override;
    void visit(const VariableOp& node) override;
    void visit(const VariableDeclaration& node) override;
    void visit(const Definition& node) override;
    void visit(const Comment& node) override;
};
