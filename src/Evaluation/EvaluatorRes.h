//
// Created by Александр Дремов on 25.11.2020.
//

#ifndef DIFFERENTIATOR_EVALUATORRES_H
#define DIFFERENTIATOR_EVALUATORRES_H

#include "Helpers/BinaryTree.h"
#include "ExprNode.h"

enum EvalStatus {
    EV_OK,
    EV_VAR_NEEDED,
    EV_MATH_TOK,
    EV_ERR
};

struct EvaluatorRes {
    EvalStatus  status;
    double      res;
};

#endif //DIFFERENTIATOR_EVALUATORRES_H
