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
struct NoOp;


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
    virtual _<> visit(const NoOp& node) = 0;
};

class ASTPrinter : public ASTVisitor {
    public:
        ASTPrinter() = default;
        int depth = 1;
        void printDepth() {
            for (int i = 0; i < depth; i++) {
                std::cout << "| ";
            }
        }
        _<> visit(const Program& node) override;
        _<> visit(const Literal& node) override;
        _<> visit(const ArithmeticOp& node) override;
        _<> visit(const LogicalOp& node) override;
        _<> visit(const StackOp& node) override;
        _<> visit(const IoOp& node) override;
        _<> visit(const Block& node) override;
        _<> visit(const IfElse& node) override;
        _<> visit(const While& node) override;
        _<> visit(const IdentifierCall& node) override;
        _<> visit(const VariableOp& node) override;
        _<> visit(const VariableDeclaration& node) override;
        _<> visit(const Definition& node) override;
        _<> visit(const NoOp& node) override;
};
