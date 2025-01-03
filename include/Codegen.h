#pragma once

#include <ASTVisitor.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>

class Codegen : public ASTVisitor {
 public:

    Codegen();

    _<> compile(const Program& program);

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
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder;
    std::unique_ptr<llvm::Module> module;
    std::unordered_map<int32_t, llvm::Value*> named_value_map;
};