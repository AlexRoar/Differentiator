//
// Created by Александр Дремов on 24.11.2020.
//

#ifndef DIFFERENTIATOR_DIFFERENTIATOR_H
#define DIFFERENTIATOR_DIFFERENTIATOR_H

#include <cctype>
#include <cstdlib>
#include <cstdio>
#include "Helpers/BinaryTree.h"
#include "ExprNode.h"
#include "Derivation/DerivativeRule.h"
#include "Evaluation/Evaluator.h"
#include "Optimization/ExprOptimizer.h"

namespace Differentiator {
    static BinaryTree<ExprNode>* derivative(BinaryTree<ExprNode>* structure, char var){
        return DerivativeRule::derivative(structure, var);
    }
};

#endif //DIFFERENTIATOR_DIFFERENTIATOR_H
