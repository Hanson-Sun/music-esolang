#include "Codegen.h"

_<> Codegen::compile(const Program& program) {
    return std::monostate();
}

_<> Codegen::visit(const Program& node) {
    return std::monostate();
}

_<> Codegen::visit(const Literal& node) {
    return std::monostate();
}

_<> Codegen::visit(const ArithmeticOp& node) {
    return std::monostate();
}

_<> Codegen::visit(const LogicalOp& node) {
    return std::monostate();
}

_<> Codegen::visit(const StackOp& node) {
    return std::monostate();
}

_<> Codegen::visit(const IoOp& node) {
    return std::monostate();
}

_<> Codegen::visit(const IfElse& node) {
    return std::monostate();
}

_<> Codegen::visit(const While& node) {
    return std::monostate();
}

_<> Codegen::visit(const Block& node) {
    return std::monostate();
}

_<> Codegen::visit(const VariableOp& node) {
    return std::monostate();
}

_<> Codegen::visit(const VariableDeclaration& node) {
    return std::monostate();
}

_<> Codegen::visit(const Definition& node) {
    return std::monostate();
}

_<> Codegen::visit(const IdentifierCall& node) {
    return std::monostate();
}

_<> Codegen::visit(const NoOp& node) {
    return std::monostate();
}