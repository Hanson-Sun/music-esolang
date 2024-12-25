#include "include/ASTVisitor.h"
#include "include/ASTNodes.h"
#include <iostream>

void ASTPrinter::visit(const Literal& node) {
    std::cout << "Literal: " << node.token.toString();
}

void ASTPrinter::visit(const ArithmeticOp& node) {
    std::cout << "ArithmeticOp: " << node.op.toString() << std::endl;
    node.left->accept(*this);
    std::cout << " - ";
    node.right->accept(*this);
}

void ASTPrinter::visit(const LogicalOp& node) {
    std::cout << "LogicalOp: " << node.op.toString() << std::endl;
    node.child->accept(*this);
}