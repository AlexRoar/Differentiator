//
// Created by Александр Дремов on 24.11.2020.
//

#ifndef DIFFERENTIATOR_DERIVATIONDSL_H
#define DIFFERENTIATOR_DERIVATIONDSL_H

#define dL derivative(node->getLeft(), var, laFile)
#define dR derivative(node->getRight(), var, laFile)
#define cL node->getLeft()->deepCopy()
#define cR node->getRight()->deepCopy()
#define CONST(VAL) constVal(VAL)

#define ADD(L, R) add(L, R)
#define MUL(L, R) mul(L, R)
#define SUB(L, R) sub(L, R)
#define DIV(L, R) div(L, R)
#define POW(L, R) pow(L, R)
#define LOG(L)    log(L)
#define SIN(L)    sin(L)
#define COS(L)    cos(L)
#define EXP(L)    exp(L)
#define SINH(L)   sinh(L)
#define COSH(L)   cosh(L)

#define DOUBLE_NODE(VAL) {                      \
    auto* newNode = BinaryTree<ExprNode>::New();\
    ExprNode nodeVal{};                         \
    nodeVal.cTor(VAL);                          \
    newNode->cTor(nodeVal, L, R);               \
    return newNode;                             \
}

#define SINGLE_NODE(VAL) {                       \
    auto *newNode = BinaryTree<ExprNode>::New(); \
    ExprNode nodeVal {};                         \
    nodeVal.cTor(VAL);                           \
    newNode->cTor(nodeVal, nullptr, R);          \
    return newNode;                              \
}

#define LA_DUMPED(expr) {   LaTEXDumper::rawWrite(laFile, LaTEXPhrases::primaryDiffStartCasesRandom()); \
                            latexDump(laFile, node);            \
                            BinaryTree<ExprNode>* result = expr;             \
                            LaTEXDumper::rawWrite(laFile, LaTEXPhrases::primaryDiffEndCasesRandom()); \
                            latexDump(laFile, result);          \
                            return result;                      \
                            }


#endif //DIFFERENTIATOR_DERIVATIONDSL_H
