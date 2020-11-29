//
// Created by Александр Дремов on 24.11.2020.
//

#ifndef DIFFERENTIATOR_DERIVATIVERULE_H
#define DIFFERENTIATOR_DERIVATIVERULE_H

#include <LaTEX/LaTEXDumper.h>
#include <ExprNode.h>
#include <Helpers/BinaryTree.h>
#include <Derivation/DerivationDSL.h>

struct DerivativeRule {
    static BinaryTree<ExprNode> *derivative(BinaryTree<ExprNode> *node, char var, FILE *laFile = nullptr) {
        if (node->getVal().getType() == TP_CST ||
            (node->getVal().getType() == TP_VAR && node->getVal().getVar() != var)) {
            return CONST(0);
        } else if (node->getVal().getType() == TP_VAR) {
            return CONST(1);
        }

        switch (node->getVal().getOperator().getCode()) {
            case OP_ADD: LA_DUMPED(ADD(dL, dR));
            case OP_SUB: LA_DUMPED(SUB(dL, dR));
            case OP_MUL: LA_DUMPED(ADD(MUL(cL, dR), MUL(dL, cR)));
            case OP_DIV: LA_DUMPED(DIV(
                    SUB(
                            MUL(dL, cR),
                            MUL(cL, dR)
                    ),
                    POW(cR, CONST(2))
            ));
            case OP_POW: LA_DUMPED(ADD(
                    MUL(
                            MUL(cR,
                                POW(cL,
                                    SUB(cR, CONST(1))
                                )
                            ),
                            dL),
                    MUL(
                            MUL(
                                    POW(cL, cR),
                                    LOG(cL)),
                            dR)
            ));
#define DEF_FUNC(OP_CODE, string, latex, eval, derivative) case OP_CODE: LA_DUMPED(derivative);

#include <Syntax/Syntax.h>

#undef DEF_FUNC
            default: {
                printf("Reached prohibited operation in %s: %s\n", __FILE__, __PRETTY_FUNCTION__);
                return CONST(0);
            }
        }
    }

    static BinaryTree<ExprNode> *add(BinaryTree<ExprNode> *L, BinaryTree<ExprNode> *R) {
        DOUBLE_NODE(OP_ADD)
    }

    static BinaryTree<ExprNode> *sub(BinaryTree<ExprNode> *L, BinaryTree<ExprNode> *R) {
        DOUBLE_NODE(OP_SUB)
    }

    static BinaryTree<ExprNode> *pow(BinaryTree<ExprNode> *L, BinaryTree<ExprNode> *R) {
        DOUBLE_NODE(OP_POW)
    }

    static BinaryTree<ExprNode> *mul(BinaryTree<ExprNode> *L, BinaryTree<ExprNode> *R) {
        DOUBLE_NODE(OP_MUL)
    }

    static BinaryTree<ExprNode> *div(BinaryTree<ExprNode> *L, BinaryTree<ExprNode> *R) {
        DOUBLE_NODE(OP_DIV)
    }

    static BinaryTree<ExprNode> *log(BinaryTree<ExprNode> *R) {
        SINGLE_NODE(OP_LOG)
    }

    static BinaryTree<ExprNode> *exp(BinaryTree<ExprNode> *R) {
        SINGLE_NODE(OP_EXP)
    }

    static BinaryTree<ExprNode> *constVal(double val) {
        auto *newNode = BinaryTree<ExprNode>::New();
        ExprNode nodeVal {};
        nodeVal.cTor(val);
        newNode->cTor(nodeVal);
        return newNode;
    }

    static BinaryTree<ExprNode> *sin(BinaryTree<ExprNode> *R) {
        SINGLE_NODE(OP_SIN)
    }

    static BinaryTree<ExprNode> *cos(BinaryTree<ExprNode> *R) {
        SINGLE_NODE(OP_COS)
    }

    static BinaryTree<ExprNode> *cosh(BinaryTree<ExprNode> *R) {
        SINGLE_NODE(OP_COSH)
    }

    static BinaryTree<ExprNode> *sinh(BinaryTree<ExprNode> *R) {
        SINGLE_NODE(OP_SINH)
    }

    static void latexDump(FILE *output, BinaryTree<ExprNode> *node) {
        if (!output)
            return;
        LaTEXDumper::dumpTreeBlock(output, node);
    }
};

#include <Derivation/DerivationDSLUndef.h>

#endif //DIFFERENTIATOR_DERIVATIVERULE_H
