#pragma once

struct Statement;
struct Program;
struct Literal;
struct ArithmeticOp;
struct LogicalOp;
struct StackOp;
struct IoOp;
struct ControlFlow;
struct Block;
struct IfElse;
struct While;
struct Identifier;
struct IdentifierCall;
struct VariableOp;
struct VariableDeclaration;
struct Definition;
struct Comment;

class ASTVisitor {
 public:
    virtual ~ASTVisitor() = default;
    virtual void visit(const Statement& node) = 0;
    virtual void visit(const Program& node) = 0;
    virtual void visit(const Literal& node) = 0;
    virtual void visit(const ArithmeticOp& node) = 0;
    virtual void visit(const LogicalOp& node) = 0;
    virtual void visit(const StackOp& node) = 0;
    virtual void visit(const IoOp& node) = 0;
    virtual void visit(const ControlFlow& node) = 0;
    virtual void visit(const Block& node) = 0;
    virtual void visit(const IfElse& node) = 0;
    virtual void visit(const While& node) = 0;
    virtual void visit(const Identifier& node) = 0;
    virtual void visit(const IdentifierCall& node) = 0;
    virtual void visit(const VariableOp& node) = 0;
    virtual void visit(const VariableDeclaration& node) = 0;
    virtual void visit(const Definition& node) = 0;
    virtual void visit(const Comment& node) = 0;
};
