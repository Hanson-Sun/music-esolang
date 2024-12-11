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
    virtual void visit(Statement& node) = 0;
    virtual void visit(Program& node) = 0;
    virtual void visit(Literal& node) = 0;
    virtual void visit(ArithmeticOp& node) = 0;
    virtual void visit(LogicalOp& node) = 0;
    virtual void visit(StackOp& node) = 0;
    virtual void visit(IoOp& node) = 0;
    virtual void visit(ControlFlow& node) = 0;
    virtual void visit(Block& node) = 0;
    virtual void visit(IfElse& node) = 0;
    virtual void visit(While& node) = 0;
    virtual void visit(Identifier& node) = 0;
    virtual void visit(IdentifierCall& node) = 0;
    virtual void visit(VariableOp& node) = 0;
    virtual void visit(VariableDeclaration& node) = 0;
    virtual void visit(Definition& node) = 0;
    virtual void visit(Comment& node) = 0;
};