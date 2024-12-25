#include "../include/ASTVisitor.h"
#include "../include/ASTNodes.h"
#include <iostream>
#include <variant>

using namespace std;

_<> ASTPrinter::visit(const Program& node) {
    int curdepth = depth;
    std::cout << "|" << "Program" << std::endl;
    for (const auto& statement : node.statements) {
        depth = curdepth + 1;
        statement->accept(*this);
    }
    depth = curdepth;
    return std::monostate{};
}

_<> ASTPrinter::visit(const Literal& node) {
    printDepth();
    std::cout << "Literal: " << node.token.toString() << std::endl;
    return std::monostate{};
}

_<> ASTPrinter::visit(const ArithmeticOp& node) {
    printDepth();
    std::cout << "ArithmeticOp: " << node.op.toString() << std::endl;
    return std::monostate{};
}

_<> ASTPrinter::visit(const LogicalOp& node) {
    printDepth();
    std::cout << "LogicalOp: " << node.op.toString() << std::endl;
    return std::monostate{};
}

_<> ASTPrinter::visit(const StackOp& node) {
    printDepth();
    std::cout << "StackOp: " << node.op.toString() << std::endl;
    return std::monostate{};
}

_<> ASTPrinter::visit(const IoOp& node) {
    printDepth();
    std::cout << "IoOp: " << node.op.toString() << std::endl;
    return std::monostate{};
}

_<> ASTPrinter::visit(const Block& node) {
    int curdepth = depth;
    for (const auto& statement : node.statements) {
        depth = curdepth;
        statement->accept(*this);
    }
    return std::monostate{};
}

_<> ASTPrinter::visit(const IfElse& node) {
    printDepth();
    std::cout << "If:" << std::endl;
    int curdepth = depth;
    depth++;
    node.then_branch->accept(*this);
    depth = curdepth+1;
    if (node.else_branch) {
        std::cout << "Else:" << std::endl;
        node.else_branch->accept(*this);
    }
    depth = curdepth;
    return std::monostate{};
}

_<> ASTPrinter::visit(const While& node) {
    printDepth();
    int curdepth = depth;
    depth++;
    std::cout << "While" << std::endl;
    node.body->accept(*this);
    depth = curdepth;
    return std::monostate{};
}

_<> ASTPrinter::visit(const IdentifierCall& node) {
    printDepth();
    std::cout << "IdentifierCall: " << node.identifier->token.toString() << std::endl;
    return std::monostate{};
}

_<> ASTPrinter::visit(const VariableOp& node) {
    printDepth();
    std::cout << "VariableOp: " << node.op.toString() << std::endl;
    return std::monostate{};
}

_<> ASTPrinter::visit(const VariableDeclaration& node) {
    printDepth();
    std::cout << "VariableDeclaration: " << node.identifier->token.toString() << std::endl;
    return std::monostate{};
}

_<> ASTPrinter::visit(const Definition& node) {
    printDepth();
    int curdepth = depth;
    depth++;
    std::cout << "Definition: " << node.identifier->token.toString() << std::endl;
    node.body->accept(*this);
    depth = curdepth;
    return std::monostate{};
}

