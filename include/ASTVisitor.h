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
    void visit(const Identifier& node) override;
    void visit(const IdentifierCall& node) override;
    void visit(const VariableOp& node) override;
    void visit(const VariableDeclaration& node) override;
    void visit(const Definition& node) override;
    void visit(const Comment& node) override;
};
