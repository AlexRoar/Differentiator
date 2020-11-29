//
// Created by Александр Дремов on 24.11.2020.
//

#ifndef DIFFERENTIATOR_EXPROPTIMIZER_H
#define DIFFERENTIATOR_EXPROPTIMIZER_H

#include <Helpers/BinaryTree.h>
#include <ExprNode.h>
#include <Evaluation/Evaluator.h>
#include <Optimization/SpecificOptimizers.h>

#define EVAL_L Evaluator::eval(head->getLeft(), evalMath)
#define EVAL_R Evaluator::eval(head->getRight(), evalMath)

#define EXPECT_EVAL_R_EQ(val) (evalR.status == EV_OK && evalR.res == val)
#define EXPECT_EVAL_L_EQ(val) (evalL.status == EV_OK && evalL.res == val)

#define LA_DUMPED(code) { \
                            unsigned before = changed; \
                            BinaryTree<ExprNode>* copy = head->deepCopy();      \
                            {code}                    \
                            if (before != changed){    \
                                LaTEXDumper::rawWrite(laFile, LaTEXPhrases::primarySimplifyStartCasesRandom()); \
                                LaTEXDumper::dumpTreeBlock(laFile, copy);       \
                                LaTEXDumper::rawWrite(laFile, LaTEXPhrases::primarySimplifyEndCasesRandom()); \
                                LaTEXDumper::dumpTreeBlock(laFile, head);       \
                            }\
                            BinaryTree<ExprNode>::Delete(copy);\
                            break;  \
                        }

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

    unsigned convTrivialExpr(BinaryTree<ExprNode> *&head, bool evalMath = false, FILE *laFile = nullptr) {
        unsigned changed = 0;
        if (head->getVal().getType() == TP_VAR || head->getVal().getType() == TP_CST)
            return 0;

        if (head->getRight())
            changed += convTrivialExpr(head->getRight(), evalMath, laFile);
        if (head->getLeft())
            changed += convTrivialExpr(head->getLeft(), evalMath, laFile);

        EvaluatorRes evalR = EVAL_R;
        EvaluatorRes evalL = EVAL_L;

        switch (head->getVal().getOperator().getCode()) {
            case OP_SUB: LA_DUMPED (changed += SpecificOptimizers::subOptimizer(head, evalL, evalR, evalMath););
            case OP_ADD: LA_DUMPED (changed += SpecificOptimizers::addOptimizer(head, evalL, evalR, evalMath););
            case OP_DIV: LA_DUMPED (changed += SpecificOptimizers::divOptimizer(head, evalL, evalR, evalMath););
            case OP_MUL: LA_DUMPED (changed += SpecificOptimizers::mulOptimizer(head, evalL, evalR, evalMath););
            case OP_POW: LA_DUMPED (changed += SpecificOptimizers::expOptimizer(head, evalL, evalR, evalMath););
#define DEF_FUNC(OP_CODE, string, latex, eval, derivative) case OP_CODE:
#include <Syntax/Syntax.h>
#undef DEF_FUNC
            {
                break;
            }
            default: {
                printf("Reached prohibited operation in %s: %s\n", __FILE__, __PRETTY_FUNCTION__);
                return 0;
            }
        }
        return changed;
    }

    unsigned convConsts(BinaryTree<ExprNode> *head, bool evalMath = false, FILE *laFile = nullptr) {
        EvaluatorRes result = Evaluator::eval(head, evalMath);
        unsigned changed = 0;
        if (result.status == EV_OK) {
            if (head->getVal().getType() == TP_CST && head->getVal().getConst() == result.res)
                return 0;
            ExprNode newValue {};
            newValue.cTor(result.res);
            LaTEXDumper::rawWrite(laFile, "Легко видно что:\n");
            LaTEXDumper::dumpTreeBlock(laFile, head);
            BinaryTree<ExprNode>::Delete(head->getLeft());
            BinaryTree<ExprNode>::Delete(head->getRight());
            head->cTor(newValue);
            LaTEXDumper::rawWrite(laFile, "преобразуется в\n");
            LaTEXDumper::dumpTreeBlock(laFile, head);
            changed += 1;
        } else {
            if (head->getLeft())
                changed += convConsts(head->getLeft(), evalMath, laFile);
            if (head->getRight())
                changed += convConsts(head->getRight(), evalMath, laFile);
        }
        return changed;
    }

public:
    static ExprOptimizer *New() {
        auto *thou = static_cast<ExprOptimizer *>(calloc(1, sizeof(ExprOptimizer)));
        thou->structure = nullptr;
        return thou;
    }

    void simplify(bool evalMath = false, FILE *laFile = nullptr) {
        unsigned changes = convConsts(structure, evalMath, laFile) + convTrivialExpr(structure, evalMath, laFile);
        while (changes) {
            changes = convConsts(structure, evalMath, laFile) + convTrivialExpr(structure, evalMath, laFile);
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

    BinaryTree<ExprNode> *getStructure() {
        return structure;
    }
};

#undef EVAL_L
#undef EVAL_R
#undef EXPECT_EVAL_R_EQ
#undef EXPECT_EVAL_L_EQ
#endif //DIFFERENTIATOR_EXPROPTIMIZER_H
