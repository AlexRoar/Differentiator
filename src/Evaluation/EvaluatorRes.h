//
// Created by Александр Дремов on 25.11.2020.
//

#ifndef DIFFERENTIATOR_EVALUATORRES_H
#define DIFFERENTIATOR_EVALUATORRES_H

#include "Helpers/BinaryTree.h"
#include "ExprNode.h"

enum EvalStatus {
    EV_OK          = 0,
    EV_VAR_NEEDED  = 1,
    EV_MATH_TOK    = 2,
    EV_ERR         = 3
};

struct EvaluatorRes {
    EvalStatus status;
    double res;
};

#endif //DIFFERENTIATOR_EVALUATORRES_H
