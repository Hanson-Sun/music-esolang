#include "Codegen.h"

#include "ASTNodes.h"
#include "Tokenizer.h"

Codegen::Codegen() {
    context = llvm::LLVMContext();
    builder = llvm::IRBuilder<>(context);
    module = std::make_unique<llvm::Module>("main", context);
    
    createPrintf();
    createScanf();
    createMemFuncs();

    creatStack();
    createArena();

    llvm::FunctionType *funcType = llvm::FunctionType::get(builder.getInt32Ty(), false);
    llvm::Function *mainFunc = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", module);

    llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "entry", mainFunc);
    builder.SetInsertPoint(entry);

}

_<> Codegen::compile(const Program& program) { 
    if (llvm::verifyModule(module, &llvm::errs())) {
        llvm::errs() << "Error: module verification failed\n";
        return ErrorHandler::createError(ErrorCode::INVALID_MODULE, "Module verification failed");
    }

    // TODO: dont know if this works
    // llvm::InitializeAllTargets();
    // llvm::InitializeAllTargetMCs();
    // llvm::InitializeAllAsmParsers();
    // llvm::InitializeAllAsmPrinters();


    // auto targetTriple = llvm::sys::getDefaultTargetTriple();
    // module.setTargetTriple(targetTriple);

    // std::string error;
    // auto target = llvm::TargetRegistry::lookupTarget(targetTriple, error);

    // if (!target) {
    //     llvm::errs() << "Error: " << error;
    //     return 1;
    // }

    // auto CPU = "generic";
    // auto features = "";

    // llvm::TargetOptions opt;
    // auto RM = llvm::Optional<llvm::Reloc::Model>();
    // auto targetMachine = target->createTargetMachine(targetTriple, CPU, features, opt, RM);

    // module.setDataLayout(targetMachine->createDataLayout());

    // std::error_code EC;
    // llvm::raw_fd_ostream dest("output.o", EC, llvm::sys::fs::OF_None);

    // if (EC) {
    //     llvm::errs() << "Could not open file: " << EC.message();
    //     return 1;
    // }

    // llvm::legacy::PassManager pass;
    // auto fileType = llvm::CGFT_ObjectFile;

    // if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, fileType)) {
    //     llvm::errs() << "The target machine can't emit a file of this type";
    //     return 1;
    // }

    // pass.run(module);
    // dest.flush();

    // llvm::outs() << "Wrote output.o\n";
    
    return std::monostate();
}

_<> Codegen::visit(const Program& node) { return std::monostate(); }

_<> Codegen::visit(const Literal& node) {
    int64_t val = node.token.value;
    llvm::Value* l_int = llvm::ConstantInt::get(context, llvm::APInt(64, val));
    push(l_int);

    return std::monostate();
}

_<> Codegen::visit(const ArithmeticOp& node) {
    // TODO: compile time error handling for stack underflow
    llvm::Value* s1 = pop();
    llvm::Value* s2 = pop();

    llvm::Value* result = nullptr;

    switch (node.op.type) {
        case TokenType::ADD: {
            result = builder.CreateAdd(s2, s1, "addtmp");
            break;
        }
        case TokenType::SUB: {
            result = builder.CreateSub(s2, s1, "subtmp");
            break;
        }
        case TokenType::MUL: {
            result = builder.CreateMul(s2, s1, "multmp");
            break;
        }
        case TokenType::DIV: {
            result = builder.CreateSDiv(s2, s1, "divtmp");
            break;
        }
        case TokenType::MOD: {
            result = builder.CreateSRem(s2, s1, "modtmp");
            break;
        }
        default:
            return ErrorHandler::createError(ErrorCode::INVALID_TOKEN,
                                             "@ Arithmetic operation: " + node.op.toString());
    }

    push(result);
    return std::monostate();
}

_<> Codegen::visit(const LogicalOp& node) {
    llvm::Value* s1 = nullptr;
    llvm::Value* s2 = nullptr;

    // TODO: fix error handling
    if (node.op.type == TokenType::NOT) {
        s1 = pop();
    } else {
        s1 = pop();
        s2 = pop();
    }

    // if (node.op.type == TokenType::NOT && stack.size() >= 1) {
    //     s1 = stack.back();
    //     stack.pop_back();
    // } else if (stack.size() >= 2) {
    //     s1 = stack.back();
    //     stack.pop_back();
    //     s2 = stack.back();
    //     stack.pop_back();
    // } else {
    //     return ErrorHandler::createError(
    //         ErrorCode::STACK_UNDERFLOW,
    //         "@ Logical operation, " + node.op.toString() + ": not enough values on stack");
    // }

    switch (node.op.type) {
        case TokenType::EQ: {
            llvm::Value* result = builder.CreateICmpEQ(s2, s1, "eqtmp");
            push(result);
            break;
        }
        case TokenType::LESS: {
            llvm::Value* result = builder.CreateICmpSLT(s2, s1, "lesstmp");
            push(result);
            break;
        }
        case TokenType::GREATER: {
            llvm::Value* result = builder.CreateICmpSGT(s2, s1, "greatertmp");
            push(result);
            break;
        }
        case TokenType::AND: {
            llvm::Value* result = builder.CreateAnd(s2, s1, "andtmp");
            push(result);
            break;
        }
        case TokenType::OR: {
            llvm::Value* result = builder.CreateOr(s2, s1, "ortmp");
            push(result);
            break;
        }
        case TokenType::NOT: {
            llvm::Value* result = builder.CreateNot(s1, "nottmp");
            push(result);
            break;
        }
        default:
            return ErrorHandler::createError(ErrorCode::INVALID_TOKEN,
                                             "@ Logical operation: " + node.op.toString());
    }

    return std::monostate();
}

_<> Codegen::visit(const StackOp& node) {
    switch (node.op.type) {
        case TokenType::POP: {
            if (auto result = pop(); _check(result))
                return ErrorHandler::addContext(result,
                                                "@ Stack pop operation: " + node.op.toString());
            break;
        }
        case TokenType::DUP: {
            auto value = top();
            if (_check(value))
                return ErrorHandler::addContext(value,
                                                "@ Stack dup operation: " + node.op.toString());

            if (auto result = push(std::get<llvm::Value*>(value)); _check(result))
                return ErrorHandler::addContext(result,
                                                "@ Stack dup operation: " + node.op.toString());

            break;
        }
        case TokenType::DUPDOT: {
            auto t = pop();
            if (_check(t))
                return ErrorHandler::addContext(t,
                                                "@ Stack dupdot operation: " + node.op.toString());

            auto llvmValue = std::get<llvm::Value*>(t);

            if (!llvm::isa<llvm::ConstantInt>(llvmValue)) {
                return ErrorHandler::createError(ErrorCode::INVALID_VALUE,
                                                 "dup. expected constant integer value");
            }

            auto constInt = llvm::cast<llvm::ConstantInt>(llvmValue);
            int64_t value = constInt->getSExtValue();

            if (value > static_cast<int64_t>(stack.size()) - 1) {
                return ErrorHandler::createError(
                    ErrorCode::STACK_UNDERFLOW,
                    "dup. stack underflow with value: " + std::to_string(value));
            }

            if (auto result = push(stack.at(stack.size() - 1 - value)); _check(result)) {
                return ErrorHandler::addContext(t, "@ Stack dup. operation: " + node.op.toString());
            }

            break;
        }
        case TokenType::SWAP: {
            auto s1 = pop();
            if (_check(s1)) {
                return ErrorHandler::addContext(s1,
                                                "@ Stack swap operation: " + node.op.toString());
            }

            auto s2 = pop();
            if (_check(s2)) {
                return ErrorHandler::addContext(s2,
                                                "@ Stack swap operation: " + node.op.toString());
            }

            if (auto result = push(std::get<llvm::Value*>(s1)); _check(result)) {
                return ErrorHandler::addContext(result,
                                                "@ Stack swap operation: " + node.op.toString());
            }

            if (auto result = push(std::get<llvm::Value*>(s2)); _check(result)) {
                return ErrorHandler::addContext(result,
                                                "@ Stack swap operation: " + node.op.toString());
            }

            break;
        }
        case TokenType::SIZE: {
            auto size = stack.size();

            llvm::Value* l_int = llvm::ConstantInt::get(context, llvm::APInt(64, size));
            if (auto result = push(l_int); _check(result)) {
                return ErrorHandler::addContext(result, "@ Stack size operation");
            }

            break;
        }
        default:
            return ErrorHandler::createError(ErrorCode::INVALID_TOKEN,
                                             "@ Stack operation: " + node.op.toString());
    };
    return std::monostate();
}

_<> Codegen::visit(const IoOp& node) {
    switch (node.op.type) {
        case TokenType::DEBUG: {
            llvm::Value* formatStr = builder.CreateGlobalStringPtr("%s ");

            for (auto it = stack.rbegin(); it != stack.rend(); ++it) {
                llvm::Value *value = *it;
                if (llvm::isa<llvm::ConstantInt>(value)) {
                    llvm::Value *args[] = { formatStr, value };
                    builder.CreateCall(printf_func, args);
                } else {
                    return ErrorHandler::createError(ErrorCode::INVALID_VALUE,
                                                     "debug expected constant integer value");
                }
            }
 
            llvm::Value *newlineStr = builder.CreateGlobalStringPtr("\n");
            llvm::Value *args[] = { newlineStr };
            builder.CreateCall(printf_func, args);
            break;
        }
        case TokenType::PRINT: {
            auto t = pop(); 
            if (_check(t))
                return ErrorHandler::addContext(t,
                                                "@ IO print operation: " + node.op.toString());

            llvm::Value* value = std::get<llvm::Value*>(t);

            if (llvm::isa<llvm::ConstantInt>(value)) {
                llvm::Value* formatStr = builder.CreateGlobalStringPtr("%ld\n");
                llvm::Value* args[] = { formatStr, value };
                builder.CreateCall(printf_func, args);
            } else {
                return ErrorHandler::createError(ErrorCode::INVALID_VALUE,
                                                 "print expected constant integer value");
            }
            break;
        }
        case TokenType::PRINTCHAR: {
            auto t = pop(); 
            if (_check(t))
                return ErrorHandler::addContext(t,
                                                "@ IO printchar operation: " + node.op.toString());

            llvm::Value* value = std::get<llvm::Value*>(t);

            if (llvm::isa<llvm::ConstantInt>(value)) {
                llvm::Value* formatStr = builder.CreateGlobalStringPtr("%c\n");
                llvm::Value* args[] = { formatStr, value };
                builder.CreateCall(printf_func, args);
            } else {
                return ErrorHandler::createError(ErrorCode::INVALID_VALUE,
                                                 "printchar expected constant integer value");
            }

            break;
        }
        case TokenType::INPUT: {
            llvm::Value* formatStr = builder.CreateGlobalStringPtr("%lld");

            llvm::Value* inputPtr = builder.CreateAlloca(llvm::Type::getInt64Ty(context), nullptr, "input");

            llvm::Value* args[] = { formatStr, inputPtr };
            builder.CreateCall(scanf_func, args);

            llvm::Value* inputValue = builder.CreateLoad(llvm::Type::getInt64Ty(context), inputPtr, "inputValue");

            if (auto result = push(inputValue); _check(result)) {
                return ErrorHandler::addContext(result, "@ IO input operation: " + node.op.toString());
            }
            break;
        }
        default:
            return ErrorHandler::createError(ErrorCode::INVALID_TOKEN,
                                             "@ IO operation: " + node.op.toString());
            break;
    }
    return std::monostate();
}

_<> Codegen::visit(const IfElse& node) { return std::monostate(); }

_<> Codegen::visit(const While& node) { return std::monostate(); }

_<> Codegen::visit(const Block& node) { 
    
    return std::monostate(); 
}

_<> Codegen::visit(const VariableOp& node) { return std::monostate(); }

_<> Codegen::visit(const VariableDeclaration& node) { return std::monostate(); }

_<> Codegen::visit(const Definition& node) { return std::monostate(); }

_<> Codegen::visit(const IdentifierCall& node) { return std::monostate(); }

_<> Codegen::visit(const NoOp& node) { return std::monostate(); }

void Codegen::createStack(std::size_t size) {
    llvm::Value* stackSize = builder.getInt64(size * sizeof(int64_t));
    llvm::Value* stackPtr = builder.CreateCall(mallocFunc, {stackSize}, "stack_ptr");

    stack = builder.CreateBitCast(stackPtr, builder.getInt64PtrTy(), "stack");
    stack_ptr = new llvm::GlobalVariable(module, builder.getInt64Ty(), false, llvm::GlobalValue::PrivateLinkage, llvm::ConstantInt::get(builder.getInt64Ty(), -1), "stack_ptr");
    stack_size = new llvm::GlobalVariable(module, builder.getInt64Ty(), false, llvm::GlobalValue::PrivateLinkage, llvm::ConstantInt::get(builder.getInt64Ty(), size), "stack_size");
}

void Codegen::createArena() {
    // TODO: figure out what i want here
}

void Codegen::createMemFuncs() {
    llvm::FunctionType* mallocType = llvm::FunctionType::get(builder.getInt8PtrTy(), {builder.getInt64Ty()}, false);
    malloc_func = llvm::Function::Create(mallocType, llvm::Function::ExternalLinkage, "malloc", module);

    llvm::FunctionType* freeType = llvm::FunctionType::get(builder.getVoidTy(), {builder.getInt8PtrTy()}, false);
    free_func = llvm::Function::Create(freeType, llvm::Function::ExternalLinkage, "free", module);

    llvm::FunctionType* reallocType =
        llvm::FunctionType::get(builder.getInt8PtrTy(), {builder.getInt8PtrTy(), builder.getInt64Ty()}, false);
    realloc_func = llvm::Function::Create(reallocType, llvm::Function::ExternalLinkage, "realloc", module.get());
}

void Codegen::createPrintf() {
    llvm::FunctionType* printfType =
        llvm::FunctionType::get(llvm::Type::getInt32Ty(context),
                                llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0), true);
    printf_func =
        llvm::Function::Create(printfType, llvm::Function::ExternalLinkage, "printf", module.get());
}

void Codegen::createScanf() {
    llvm::FunctionType* scanfType =
        llvm::FunctionType::get(llvm::Type::getInt32Ty(context),
                                llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0), true);
    scanf_func =
        llvm::Function::Create(scanfType, llvm::Function::ExternalLinkage, "scanf", module.get());
}

llvm::Value* Codegen::pop() {
    llvm::Value* sp = builder.CreateLoad(stack_ptr, "sp");
    llvm::Value* currStackSize = builder.CreateLoad(stack_size, "currStackSize");

    // check if stack is empty
    llvm::Value* stackEmpty = builder.CreateICmpEQ(sp, builder.getInt64(-1), "stackEmpty");
    llvm::Function* currentFunction = builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* popBlock = llvm::BasicBlock::Create(context, "pop", currentFunction);
    llvm::BasicBlock* errorBlock = llvm::BasicBlock::Create(context, "error", currentFunction);
    // if empty, error, else pop
    builder.CreateCondBr(stackEmpty, errorBlock, popBlock);

    // error block
    builder.SetInsertPoint(errorBlock);
    builder.CreateCall(printf_func, {builder.CreateGlobalStringPtr("Error: Stack underflow\n")});
    builder.CreateRetVoid();

    // pop block
    builder.SetInsertPoint(popBlock);
    llvm::Value* newSp = builder.CreateSub(sp, builder.getInt64(1), "newSp");
    builder.CreateStore(newSp, stack_ptr);
    // load popped value
    llvm::Value* elemPtr = builder.CreateGEP(stack, newSp, "elemPtr");
    llvm::Value* value = builder.CreateLoad(elemPtr, "value");

    return value;
}

void Codegen::push(llvm::Value* value) {
    // curr stack values
    llvm::Value* sp = builder.CreateLoad(stack_ptr, "sp");
    llvm::Value* currStackSize = builder.CreateLoad(stack_size, "currStackSize");

    // check if stack is full
    llvm::Value* stackFull = builder.CreateICmpEQ(sp, currStackSize, "stackFull");

    // if full, realloc, else push
    llvm::Function* currentFunction = builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* pushBlock = llvm::BasicBlock::Create(context, "push", currentFunction);
    llvm::BasicBlock* reallocBlock = llvm::BasicBlock::Create(context, "realloc", currentFunction);
    builder.CreateCondBr(stackFull, reallocBlock, pushBlock);

    // realloc block
    {
        builder.SetInsertPoint(reallocBlock);
        llvm::Value* newSize = builder.CreateMul(currStackSize, builder.getInt64(2), "newSize");
        llvm::Value* newStackPtr = builder.CreateCall(realloc_func, {builder.CreateBitCast(stack, builder.getInt8PtrTy()), newSize}, "newStackPtr");
        stack = builder.CreateBitCast(newStackPtr, builder.getInt64PtrTy(), "stack");
        builder.CreateStore(newSize, stack_size);
        builder.CreateBr(pushBlock);
    }

    // push block
    {
        builder.SetInsertPoint(pushBlock);
        llvm::Value* elemPtr = builder.CreateGEP(stack, sp, "elemPtr");
        builder.CreateStore(value, elemPtr);
        // update stack ptr
        llvm::Value* newSp = builder.CreateAdd(sp, builder.getInt64(1), "newSp");
        builder.CreateStore(newSp, stack_ptr);
    }

    return std::monostate();
}

llvm::Value* Codegen::top() {
    llvm::Value* currStackSize = builder.CreateLoad(stack_size, "currStackSize");
    llvm::Value* sp = builder.CreateLoad(stack_ptr, "sp");


    // check if stack is empty
    llvm::Value* stackEmpty = builder.CreateICmpEQ(sp, builder.getInt64(-1), "stackEmpty");
    llvm::Function* currentFunction = builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* topBlock = llvm::BasicBlock::Create(context, "top", currentFunction);
    llvm::BasicBlock* errorBlock = llvm::BasicBlock::Create(context, "error", currentFunction);
    builder.CreateCondBr(stackEmpty, errorBlock, topBlock);

    // error block
    builder.SetInsertPoint(errorBlock);
    builder.CreateCall(printf_func, {builder.CreateGlobalStringPtr("Error: Stack underflow\n")});
    builder.CreateRetVoid();

    // top block
    builder.SetInsertPoint(topBlock);
    llvm::Value* sp = builder.CreateLoad(stack_ptr, "sp");
    llvm::Value* elemPtr = builder.CreateGEP(stack, sp, "elemPtr");

    llvm::Value* value = builder.CreateLoad(elemPtr, "top");

    return value;
}