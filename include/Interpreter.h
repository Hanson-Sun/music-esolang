#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <queue>

#include "ASTVisitor.h"
#include "ASTNodes.h"
#include "Errors.h"

class Interpreter : public ASTVisitor {
 public:
    Interpreter() {
      // main scope
      enterScope();
    }

    _<> interpret(Statement_t statement);
    _<> interpret(Program_t program);

    _<> visit(const Program& node) override;
    _<> visit(const Literal& node) override;
    _<> visit(const ArithmeticOp& node) override;
    _<> visit(const LogicalOp& node) override;
    _<> visit(const StackOp& node) override;
    _<> visit(const IoOp& node) override;
    _<> visit(const IfElse& node) override;
    _<> visit(const While& node) override;
    _<> visit(const Block& node) override;
    _<> visit(const VariableOp& node) override;
    _<> visit(const VariableDeclaration& node) override;
    _<> visit(const Definition& node) override;
    _<> visit(const IdentifierCall& node) override;
    _<> visit(const NoOp& node) override;

 private:

    std::vector<int64_t> stack;
    
    // okay so a stack of scopes
    std::vector<std::vector<int64_t>> scopes;  

    std::unordered_map<int64_t, Block_t> func_defs;
    std::unordered_map<int64_t, int64_t> var_defs;

    // arena for var op creation
    std::vector<int64_t> arena;
    std::queue<int64_t> arena_free; // queue of available addresses
    std::vector<bool> arena_used;  // true if address is used

    _<int64_t> a_malloc();
    _<>        a_free(int64_t address);
    _<int64_t> a_at(int64_t address);  // get value at address
    _<>        a_set(int64_t address, int64_t value);

    _<>        push(int64_t value);
    _<int64_t> pop();
    _<int64_t> top();

    _<> enterScope();
    _<> exitScope();

    _<> getVar(int64_t varName);
    _<> setVar(int64_t varName, int64_t value);

    _<> getFunc(int64_t funcName);
    _<> setFunc(int64_t funcName, const Block_t def);

    // probably need a bunch of other helper functions
    // also figure out how to do scoping
};
