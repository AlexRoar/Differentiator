//
// Created by Александр Дремов on 24.11.2020.
//

#ifndef DIFFERENTIATOR_EXPROPTIMIZER_H
#define DIFFERENTIATOR_EXPROPTIMIZER_H

#include "Helpers/BinaryTree.h"
#include "ExprNode.h"
#include "Evaluation/Evaluator.h"

#define EVAL_L Evaluator::eval(head->getLeft(), evalMath)
#define EVAL_R Evaluator::eval(head->getRight(), evalMath)

#define EXPECT_EVAL_R_EQ(val) (evalR.status == EV_OK && evalR.res == val)
#define EXPECT_EVAL_L_EQ(val) (evalL.status == EV_OK && evalL.res == val)

class ExprOptimizer {
    BinaryTree<ExprNode> *structure;

    bool treeHasSpecificVariable(BinaryTree<ExprNode> *head, char var) {
        bool res = false;

        if (head->getVal().getType() == TP_VAR && head->getVal().getVar() == var) {
            return true;
        }
        if (head->getLeft()) {
            res |= treeHasSpecificVariable(head->getLeft(), var);
        }
        if (head->getRight()) {
            res |= treeHasSpecificVariable(head->getRight(), var);
        }
        return res;
    }

    bool treeHasAnyVariable(BinaryTree<ExprNode> *head) {
        bool res = false;

        if (head->getVal().getType() == TP_VAR) {
            return true;
        }
        if (head->getLeft()) {
            res |= treeHasAnyVariable(head->getLeft());
        }
        if (head->getRight()) {
            res |= treeHasAnyVariable(head->getRight());
        }
        return res;
    }

    unsigned convTrivialExpr(BinaryTree<ExprNode> *&head, bool evalMath=false) {
        unsigned changed = 0;
        if (head->getVal().getType() == TP_VAR || head->getVal().getType() == TP_CST)
            return changed;

        if (head->getRight())
            changed += convTrivialExpr(head->getRight());
        if (head->getLeft())
            changed += convTrivialExpr(head->getLeft());

        EvaluatorRes evalR = EVAL_R;
        EvaluatorRes evalL = EVAL_L;
        switch (head->getVal().getOperator().getCode()) {
            case OP_SUB: {
                if (EXPECT_EVAL_R_EQ(0)) {
                    BinaryTree<ExprNode>::Delete(head->getRight());
                    head = head->getLeft();
                    changed += 1;
                }
                break;
            }
            case OP_ADD: {
                if (EXPECT_EVAL_R_EQ(0)) {
                    BinaryTree<ExprNode>::Delete(head->getRight());
                    head = head->getLeft();
                    changed += 1;
                } else if (EXPECT_EVAL_L_EQ(0)) {
                    BinaryTree<ExprNode>::Delete(head->getLeft());
                    head = head->getRight();
                    changed += 1;
                }
                break;
            }
            case OP_DIV:
                if (EXPECT_EVAL_R_EQ(1)) {
                    BinaryTree<ExprNode>::Delete(head->getRight());
                    head = head->getLeft();
                    changed += 1;
                }
                break;
            case OP_MUL: {
                if (EXPECT_EVAL_R_EQ(1)) {
                    BinaryTree<ExprNode>::Delete(head->getRight());
                    head = head->getLeft();
                    changed += 1;
                } else if (EXPECT_EVAL_L_EQ(1)) {
                    BinaryTree<ExprNode>::Delete(head->getLeft());
                    head = head->getRight();
                    changed += 1;
                } else if ( EXPECT_EVAL_R_EQ(0) || EXPECT_EVAL_L_EQ(0)) {
                    ExprNode newVal {};
                    newVal.cTor(0.0);
                    BinaryTree<ExprNode>::Delete(head->getLeft());
                    BinaryTree<ExprNode>::Delete(head->getRight());
                    head->cTor(newVal);
                    changed += 1;
                }
                break;
            }
            case OP_EXP: {
                if (EXPECT_EVAL_R_EQ(1)) {
                    BinaryTree<ExprNode>::Delete(head->getRight());
                    head = head->getLeft();
                    changed += 1;
                } else if (EXPECT_EVAL_R_EQ(0)) {
                    ExprNode newVal {};
                    newVal.cTor(1.0);
                    BinaryTree<ExprNode>::Delete(head->getLeft());
                    BinaryTree<ExprNode>::Delete(head->getRight());
                    head->cTor(newVal);
                    changed += 1;
                }
                break;
            }
            case OP_SIN:
            case OP_COS:
            case OP_TAN:
            case OP_CTG:
            case OP_ATAN:
            case OP_ACOS:
            case OP_ASIN:
            case OP_ACTG:
            case OP_LOG:{
                break;
            }
            default: {
                printf("Reached prohibited operation in %s: %s\n", __FILE__, __PRETTY_FUNCTION__ );
                return 0;
            }
        }
        return changed;
    }

    unsigned convConsts(BinaryTree<ExprNode> *head, bool evalMath=false) {
        EvaluatorRes result = Evaluator::eval(head, evalMath);
        unsigned changed = 0;
        if (result.status == EV_OK) {
            if (head->getVal().getType() == TP_CST && head->getVal().getConst() == result.res)
                return 0;
            ExprNode newValue {};
            newValue.cTor(result.res);
            BinaryTree<ExprNode>::Delete(head->getLeft());
            BinaryTree<ExprNode>::Delete(head->getRight());
            head->cTor(newValue);
            changed += 1;
        } else {
            if (head->getLeft())
                changed += convConsts(head->getLeft(), evalMath);
            if (head->getRight())
                changed += convConsts(head->getRight(), evalMath);
        }
        return changed;
    }

public:
    static ExprOptimizer *New() {
        auto *thou = static_cast<ExprOptimizer *>(calloc(1, sizeof(ExprOptimizer)));
        thou->structure = nullptr;
        return thou;
    }

    void simplify(bool evalMath=false) {
        unsigned changes = convConsts(structure, evalMath) + convTrivialExpr(structure, evalMath);
        while (changes) {
            changes = convConsts(structure, evalMath) + convTrivialExpr(structure, evalMath);
        }
    }

    void cTor(BinaryTree<ExprNode> *newStructure) {
        this->structure = newStructure;
    }

    void dTor() {}

    static void Delete(ExprOptimizer *obj) {
        obj->dTor();
        free(obj);
    }

    BinaryTree<ExprNode> * getStructure(){
        return structure;
    }
};

#undef EVAL_L
#undef EVAL_R
#undef EXPECT_EVAL_R_EQ
#undef EXPECT_EVAL_L_EQ
#endif //DIFFERENTIATOR_EXPROPTIMIZER_H
