#pragma once

#include <stack>
#include <unordered_map>
#include <vector>
#include <string>

#include "ASTNodes.h"
#include "ASTVisitor.h"

class Interpreter : public ASTVisitor {
 public:
    void interpret(Program_t program);

    void visit(const Program& node) override;
    void visit(const Literal& node) override;
    void visit(const ArithmeticOp& node) override;
    void visit(const LogicalOp& node) override;
    void visit(const StackOp& node) override;
    void visit(const IoOp& node) override;
    void visit(const ControlFlow& node) override;
    void visit(const IfElse& node) override;
    void visit(const While& node) override;
    void visit(const Block& node) override;
    void visit(const VariableOp& node) override;
    void visit(const Identifier& node) override;
    void visit(const IdentifierCall& node) override;
    void visit(const Definition& node) override;
    void visit(const Comment& node) override;

 private:
    std::stack<int> stack;
    std::vector<std::unordered_map<std::string, int>> scopes;
    std::unordered_map<std::string, Definition*> functionDefinitions;

    void push(int value);
    int pop();
    void enterScope();
    void exitScope();

    // probably need a bunch of other helper functions
    // also figure out how to do scoping
};
