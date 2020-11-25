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

class Differentiator {
    BinaryTree<ExprNode>* structure;
public:
    static Differentiator* New() {
        auto* thou  = static_cast<Differentiator*>(calloc(1, sizeof(Differentiator)));
        thou->structure  = nullptr;
        return thou;
    }

    void cTor(BinaryTree<ExprNode>* newStructure) {
        this->structure = newStructure;
    }

    void dTor(){
    }

    static void Delete(Differentiator* obj) {
        obj->dTor();
        free(obj);
    }

    BinaryTree<ExprNode>* derivative(char var){
        return DerivativeRule::derivative(structure, var);
    }
};

#endif //DIFFERENTIATOR_DIFFERENTIATOR_H
