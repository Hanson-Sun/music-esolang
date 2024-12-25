#include "include/ASTVisitor.h"
#include "include/ASTNodes.h"
#include <iostream>
#include <variant>

using namespace std;

std::monostate ASTPrinter::visit(const Program& node) {
    int curdepth = depth;
    std::cout << "|" << "Program" << std::endl;
    for (const auto& statement : node.statements) {
        depth = curdepth + 1;
        statement->accept(*this);
    }
    depth = curdepth;
    return std::monostate{};
}

std::monostate ASTPrinter::visit(const Literal& node) {
    printDepth();
    std::cout << "Literal: " << node.token.chordLexeme << std::endl;
    return std::monostate{};
}

std::monostate ASTPrinter::visit(const ArithmeticOp& node) {
    printDepth();
    std::cout << "ArithmeticOp: " << node.op.chordLexeme << std::endl;
    return std::monostate{};
}

std::monostate ASTPrinter::visit(const LogicalOp& node) {
    printDepth();
    std::cout << "LogicalOp: " << node.op.chordLexeme << std::endl;
    return std::monostate{};
}

std::monostate ASTPrinter::visit(const StackOp& node) {
    printDepth();
    std::cout << "StackOp: " << node.op.chordLexeme << std::endl;
    return std::monostate{};
}

std::monostate ASTPrinter::visit(const IoOp& node) {
    printDepth();
    std::cout << "IoOp: " << node.op.chordLexeme << std::endl;
    return std::monostate{};
}

std::monostate ASTPrinter::visit(const Block& node) {
    printDepth();
    for (const auto& statement : node.statements) {
        statement->accept(*this);
    }
    return std::monostate{};
}

std::monostate ASTPrinter::visit(const IfElse& node) {
    printDepth();
    std::cout << "IfElse" << std::endl;
    int curdepth = depth;
    depth++;
    node.then_branch->accept(*this);
    depth = curdepth+1;
    if (node.else_branch) {
        node.else_branch->accept(*this);
    }
    depth = curdepth;
    return std::monostate{};
}

std::monostate ASTPrinter::visit(const While& node) {
    printDepth();
    int curdepth = depth;
    depth++;
    std::cout << "While" << std::endl;
    node.body->accept(*this);
    depth = curdepth;
    return std::monostate{};
}

std::monostate ASTPrinter::visit(const IdentifierCall& node) {
    printDepth();
    std::cout << "IdentifierCall: " << node.identifier->token.chordLexeme << std::endl;
    return std::monostate{};
}

std::monostate ASTPrinter::visit(const VariableOp& node) {
    printDepth();
    std::cout << "VariableOp: " << node.op.chordLexeme << std::endl;
    return std::monostate{};
}

std::monostate ASTPrinter::visit(const VariableDeclaration& node) {
    printDepth();
    std::cout << "VariableDeclaration: " << node.identifier->token.chordLexeme << std::endl;
    return std::monostate{};
}

std::monostate ASTPrinter::visit(const Definition& node) {
    printDepth();
    int curdepth = depth;
    depth++;
    std::cout << "Definition: " << node.identifier->token.chordLexeme << std::endl;
    node.body->accept(*this);
    depth = curdepth;
    return std::monostate{};
}

