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
