//
// Created by Александр Дремов on 24.11.2020.
//

#ifndef DIFFERENTIATOR_EVALUATOR_H
#define DIFFERENTIATOR_EVALUATOR_H

#include <cctype>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include "Helpers/BinaryTree.h"
#include "ExprNode.h"
#include "Derivation/DerivativeRule.h"
#include "Evaluation/EvaluatorRes.h"

#define EVAL_L eval(head->getLeft(), evalMath)
#define EVAL_R eval(head->getRight(), evalMath)

#define CONV_MATH(expr) (floor(expr) == expr || evalMath)? \
                                    EvaluatorRes {EV_OK, expr} :            \
                                    EvaluatorRes {EV_MATH_TOK, 0}

namespace Evaluator {
    static EvaluatorRes funcProcess(double calc, bool evalMath) {
        if (floor(calc) == calc || evalMath)
            return EvaluatorRes {EV_OK, calc};
        return EvaluatorRes {EV_MATH_TOK, 0};
    }

    static EvaluatorRes
    eval(BinaryTree<ExprNode> *head, bool evalMath = false, bool evalVar = false, const double *vars = nullptr) {
        if (!head)
            return EvaluatorRes {EV_ERR, 0};
        if (head->getVal().getType() == TP_VAR) {
            if (evalVar) {
                return EvaluatorRes {EV_OK, vars[head->getVal().getVar() - 'a']};
            }
            return EvaluatorRes {EV_VAR_NEEDED, 0};
        } else if (head->getVal().getType() == TP_CST)
            return EvaluatorRes {EV_OK, head->getVal().getConst()};

        EvaluatorRes evalR = EVAL_R;
        auto evalL = EvaluatorRes {EV_ERR, 0};
        if (head->getVal().getOperator().getArgsCount() == 2)
            evalL = EVAL_L;

        if (evalL.status != EV_OK)
            return evalL;

        if (evalR.status != EV_OK)
            return evalR;

        switch (head->getVal().getOperator().getCode()) {
            case OP_SUB:
                return CONV_MATH(evalL.res - evalR.res);
            case OP_ADD:
                return CONV_MATH(evalL.res + evalR.res);
            case OP_DIV:
                return CONV_MATH(evalL.res / evalR.res);
            case OP_MUL:
                return CONV_MATH(evalL.res * evalR.res);
            case OP_EXP:
                return funcProcess(pow(evalL.res, evalR.res), evalMath);
            case OP_SIN:
                return funcProcess(sin(evalR.res), evalMath);
            case OP_COS:
                return funcProcess(cos(evalR.res), evalMath);
            case OP_TAN:
                return funcProcess(tan(evalR.res), evalMath);
            case OP_CTG:
                return funcProcess(1 / tan(evalR.res), evalMath);
            case OP_ATAN:
                return funcProcess(atan(evalR.res), evalMath);
            case OP_ACOS:
                return funcProcess(acos(evalR.res), evalMath);
            case OP_ASIN:
                return funcProcess(asin(evalR.res), evalMath);
            case OP_ACTG:
                return funcProcess(atan(1 / evalR.res), evalMath);
            case OP_LOG:
                return funcProcess(log(evalR.res), evalMath);
            default: {
                printf("Reached prohibited operation in %s: %s\n", __FILE__, __PRETTY_FUNCTION__);
                return EvaluatorRes {EV_ERR, 0};
            }
        }
    }
};

#undef EVAL_L
#undef EVAL_R
#endif //DIFFERENTIATOR_DIFFERENTIATOR_H
