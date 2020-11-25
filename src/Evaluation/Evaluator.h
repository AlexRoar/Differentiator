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

class Evaluator {
    BinaryTree<ExprNode>* structure;
public:
    static EvaluatorRes eval(BinaryTree<ExprNode>* head, bool evalMath=false, bool evalVar=false, const double* vars= nullptr){
        if (!head)
            return EvaluatorRes {EV_ERR, 0};
        if (head->getVal().getType() == TP_VAR)
            return EvaluatorRes{EV_VAR_NEEDED, 0};
        else if (head->getVal().getType() == TP_CST)
            return EvaluatorRes{EV_OK, head->getVal().getConst()};

        EvaluatorRes evalR = EVAL_R;

        auto evalL = EvaluatorRes{EV_OK, 0};
        if (head->getVal().getOperator().getArgsCount() == 2)
            evalL = EVAL_L;

        if (evalL.status != EV_OK)
            return evalL;

        if (evalR.status != EV_OK)
            return evalR;

        switch(head->getVal().getOperator().getCode()){
            case OP_SUB:
                return EvaluatorRes{EV_OK, evalL.res - evalR.res};
            case OP_ADD:
                return EvaluatorRes{EV_OK, evalL.res + evalR.res};
            case OP_DIV:
                return EvaluatorRes{EV_OK, evalL.res / evalR.res};
            case OP_MUL:
                return EvaluatorRes{EV_OK, evalL.res * evalR.res};
            case OP_EXP: {
                double calc = pow(evalL.res, evalR.res);
                if (floor(calc) == calc || evalMath)
                    return EvaluatorRes {EV_OK, calc};
                return EvaluatorRes {EV_MATH_TOK, 0};
            }
            case OP_SIN: {
                double calc = sin(evalR.res);
                if (floor(calc) == calc || evalMath)
                    return EvaluatorRes {EV_OK, calc};
                return EvaluatorRes {EV_MATH_TOK, 0};
            }
            case OP_COS: {
                double calc = cos(evalR.res);
                if (floor(calc) == calc || evalMath)
                    return EvaluatorRes {EV_OK, calc};
                return EvaluatorRes {EV_MATH_TOK, 0};
            }
            case OP_TAN: {
                double calc = tan(evalR.res);
                if (floor(calc) == calc || evalMath)
                    return EvaluatorRes {EV_OK, calc};
                return EvaluatorRes {EV_MATH_TOK, 0};
            }
            case OP_CTG: {
                double calc = 1 / tan(evalR.res);
                if (floor(calc) == calc || evalMath)
                    return EvaluatorRes {EV_OK, calc};
                return EvaluatorRes {EV_MATH_TOK, 0};
            }
            case OP_ATAN: {
                double calc = atan(evalR.res);
                if (floor(calc) == calc || evalMath)
                    return EvaluatorRes {EV_OK, calc};
                return EvaluatorRes {EV_MATH_TOK, 0};
            }
            case OP_ACOS: {
                double calc = acos(evalR.res);
                if (floor(calc) == calc || evalMath)
                    return EvaluatorRes {EV_OK, calc};
                return EvaluatorRes {EV_MATH_TOK, 0};
            }
            case OP_ASIN:{
                double calc = asin(evalR.res);
                if (floor(calc) == calc || evalMath)
                    return EvaluatorRes {EV_OK, calc};
                return EvaluatorRes {EV_MATH_TOK, 0};
            }
            case OP_ACTG:{
                double calc = atan(1 / evalR.res);
                if (floor(calc) == calc || evalMath)
                    return EvaluatorRes {EV_OK, calc};
                return EvaluatorRes {EV_MATH_TOK, 0};
            }
            case OP_LOG:{
                double calc = log(evalR.res);
                if (floor(calc) == calc || evalMath)
                    return EvaluatorRes {EV_OK, calc};
                return EvaluatorRes {EV_MATH_TOK, 0};
            }
            default: {
                printf("Reached prohibited operation in %s: %s\n", __FILE__, __PRETTY_FUNCTION__ );
                return EvaluatorRes {EV_ERR, 0};
            }
        }
    }

    static Evaluator* New() {
        auto* thou  = static_cast<Evaluator*>(calloc(1, sizeof(Evaluator)));
        thou->structure  = nullptr;
        return thou;
    }

    void cTor(BinaryTree<ExprNode>* newStructure) {
        this->structure = newStructure;
    }

    void dTor(){}

    static void Delete(Evaluator* obj) {
        obj->dTor();
        free(obj);
    }
};

#undef EVAL_L
#undef EVAL_R
#endif //DIFFERENTIATOR_DIFFERENTIATOR_H
