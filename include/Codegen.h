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

    // i think i need to handle this differently
    std::unordered_map<int32_t, llvm::Value*> named_value_map; 

    llvm::Function* printf_func;
    llvm::Function* scanf_func;
    llvm::Function* malloc_func;
    llvm::Function* realloc_func;
    llvm::Function* free_func;

    llvm::Value*          stack;      // stack 
    llvm::GlobalVariable* stack_ptr;  // index
    llvm::GlobalVariable* stack_size; // integer size

    llvm::Value*          arena; // i think i need extra stuff here :frown:
    llvm::GlobalVariable* arena_size;

    void createStack(std::size_t size = 1024);
    void createArena();
    void createMemFuncs();
    void createPrintf();
    void createScanf();

    void         push(llvm::Value* value);
    llvm::Value* pop();
    llvm::Value* top();

    _<> enterScope();
    _<> exitScope();

    //  _<> getVar(int64_t varName);
    //  _<> setVar(int64_t varName, int64_t value);

    //  _<> getFunc(int64_t funcName);
    //  _<> setFunc(int64_t funcName, const Block_t def);
};