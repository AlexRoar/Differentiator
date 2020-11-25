//
// Created by Александр Дремов on 24.11.2020.
//

#ifndef DIFFERENTIATOR_DERIVATIONDSL_H
#define DIFFERENTIATOR_DERIVATIONDSL_H

#define dL derivative(node->getLeft(), var)
#define dR derivative(node->getRight(), var)
#define cL node->getLeft()->deepCopy()
#define cR node->getRight()->deepCopy()
#define CONST(VAL) constVal(VAL)

#define ADD(L, R) add(L, R)
#define MUL(L, R) mul(L, R)
#define SUB(L, R) sub(L, R)
#define DIV(L, R) div(L, R)
#define POW(L, R) pow(L, R)
#define LOG(L) log(L)
#define SIN(L) sin(L)
#define COS(L) cos(L)

#define DOUBLE_NODE(VAL) {                      \
    auto* newNode = BinaryTree<ExprNode>::New();\
    ExprNode nodeVal{};                         \
    nodeVal.cTor(VAL);                          \
    newNode->cTor(nodeVal, L, R);               \
    return newNode;                             \
}


#endif //DIFFERENTIATOR_DERIVATIONDSL_H
