//
// Created by Александр Дремов on 24.11.2020.
//

#ifndef DIFFERENTIATOR_DIFFERENTIATOR_H
#define DIFFERENTIATOR_DIFFERENTIATOR_H

#include <cctype>
#include <cstdlib>
#include <cstdio>
#include <Helpers/InfixParser.h>
#include <Helpers/BinaryTree.h>
#include <ExprNode.h>
#include <Derivation/DerivativeRule.h>
#include <Evaluation/Evaluator.h>
#include <Optimization/ExprOptimizer.h>

namespace Differentiator {
    static BinaryTree<ExprNode>* derivative(BinaryTree<ExprNode>* structure, char var, FILE* laFile= nullptr){
        return DerivativeRule::derivative(structure, var, laFile);
    }

    void ArticleGenerator(FILE *output, char *content) {
        LaTEXDumper::dumpDocStart(output);

        auto* graph = InfixParser::parseExpression(content);
        LaTEXDumper::dumpTreeBlock(output, graph);

        LaTEXDumper::rawWrite(output, LaTEXPhrases::primarySimplify);
        ExprOptimizer optimizer{};
        optimizer.cTor(graph);
        optimizer.simplify(false, output);

        LaTEXDumper::rawWrite(output, LaTEXPhrases::primarySimplifyEnd);
        graph = optimizer.getStructure();
        graph->dumpGraph();
        LaTEXDumper::dumpTreeBlock(output, graph);

        LaTEXDumper::rawWrite(output, LaTEXPhrases::primaryDiff);

        auto* derived = Differentiator::derivative(graph, 'x', output);
        LaTEXDumper::rawWrite(output, LaTEXPhrases::primaryDiffEnd);
        LaTEXDumper::dumpDiffResult(output, content, derived);
        optimizer.cTor(derived);

        LaTEXDumper::rawWrite(output, LaTEXPhrases::secondSimplify);
        optimizer.simplify(false, output);
        derived = optimizer.getStructure();
        LaTEXDumper::rawWrite(output, LaTEXPhrases::secondSimplifyEnd);
        LaTEXDumper::dumpDiffResult(output, content, derived);

        LaTEXDumper::dumpDocEnd(output);
    }
};

#endif //DIFFERENTIATOR_DIFFERENTIATOR_H
