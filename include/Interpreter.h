#pragma once

#include <stack>
#include <unordered_map>
#include <vector>
#include <string>

#include "ASTNodes.h"
#include "ASTVisitor.h"

class Interpreter : public ASTVisitor {
 public:
    void interpret(Program* program);

    void visit(Program& node) override;
    void visit(Literal& node) override;
    void visit(ArithmeticOp& node) override;
    void visit(LogicalOp& node) override;
    void visit(StackOp& node) override;
    void visit(IoOp& node) override;
    void visit(ControlFlow& node) override;
    void visit(IfElse& node) override;
    void visit(While& node) override;
    void visit(Block& node) override;
    void visit(VariableOp& node) override;
    void visit(Identifier& node) override;
    void visit(IdentifierCall& node) override;
    void visit(Definition& node) override;
    void visit(Comment& node) override;

 private:
    std::stack<int> stack_;
    std::vector<std::unordered_map<std::string, int>> scopes_;         
    std::unordered_map<std::string, Definition*> functionDefinitions; 

    void push(int value);
    int pop();
    void enterScope();
    void exitScope();

    // probably need a bunch of other helper functions
    // also figure out how to do scoping
};
