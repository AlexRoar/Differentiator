//
// Created by Александр Дремов on 24.11.2020.
//

#ifndef DIFFERENTIATOR_DIFFERENTIATOR_H
#define DIFFERENTIATOR_DIFFERENTIATOR_H

#include <cctype>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <Helpers/InfixParser.h>
#include <Helpers/BinaryTree.h>
#include <ExprNode.h>
#include <Derivation/DerivativeRule.h>
#include <Evaluation/Evaluator.h>
#include <Optimization/ExprOptimizer.h>

namespace Differentiator {
    static int variablesMaxNum = 26 * 2;

    static BinaryTree<ExprNode> *derivative(BinaryTree<ExprNode> *structure, char var, FILE *laFile = nullptr) {
        return DerivativeRule::derivative(structure, var, laFile);
    }

    static BinaryTree<ExprNode> **
    derivativeFull(BinaryTree<ExprNode> *structure, const char *variables, size_t& varCounter, FILE *laFile = nullptr) {
        size_t varsNum = 0;
        for(unsigned i = 0; i < variablesMaxNum; ++i)
            varsNum+=variables[i];

        auto** result = static_cast<BinaryTree<ExprNode> **>(calloc(varsNum, sizeof(BinaryTree<ExprNode> *)));

        varCounter = 0;
        for(unsigned i = 0; i < variablesMaxNum; ++i) {
            if (variables[i] == 0)
                continue;
            if (laFile)
                fprintf(laFile, "\\textbf{Рассчитаем частную производную} $$\\frac{\\partial f}{\\partial %c} $$", char(i + 'a'));
            result[varCounter++] = DerivativeRule::derivative(structure, char(i + 'a'), laFile);
        }
        return result;
    }

    static char */*[26*2]*/ expressionVariables(ExprNode *content, size_t len) {
        char *variables = static_cast<char *>(calloc(variablesMaxNum, sizeof(char)));

        for (unsigned i = 0; i < len; ++i) {
            if (content[i].getType() == TP_VAR) {
                variables[content[i].getVar() - 'a'] = 1;
            }
        }

        return variables;
    }

    static void dumpFullDerivative(FILE *output, const char *vars, BinaryTree<ExprNode> **partialDerivatives) {
        size_t varCounter = 0;
        fprintf(output, "\\[");
        for(unsigned i = 0; i < variablesMaxNum; ++i) {
            if (vars[i] == 0)
                continue;
            if (varCounter != 0)
                fprintf(output, " + ");
            LaTEXDumper::dumpPartial(output, partialDerivatives[varCounter++], char(i + 'a'));

        }
        fprintf(output, "\\]");
    }

    static BinaryTree<ExprNode> ** fullDerivativeSimplify(BinaryTree<ExprNode> **partialDerivatives, size_t varNum, FILE *laDumped=nullptr) {
        auto** result = static_cast<BinaryTree<ExprNode> **>(calloc(varNum, sizeof(BinaryTree<ExprNode> *)));
        for(unsigned i = 0; i < varNum; ++i) {
            ExprOptimizer optimizer {};
            optimizer.cTor(partialDerivatives[i]);
            optimizer.simplify(false, laDumped);
            result[i] = optimizer.getStructure();
            optimizer.dTor();
        }

        return result;
    }

    void ArticleGenerator(FILE *output, char *content) {
        LaTEXDumper::dumpDocStart(output);

        ClassicStack<ExprNode>* tokens = nullptr;
        auto *graph = InfixParser::parseExpression(content, &tokens);
        auto* initialGraph = graph;
        if (graph == nullptr) {
            printf("Invalid expression!\n");
            return;
        }

        LaTEXDumper::dumpTreeBlock(output, graph);
        graph->dumpGraph("parsed.svg");

        LaTEXDumper::rawWrite(output, LaTEXPhrases::getPrimarySimplify());

        ExprOptimizer optimizer {};
        optimizer.cTor(graph);
        optimizer.simplify(false, output);

        LaTEXDumper::rawWrite(output, LaTEXPhrases::getPrimarySimplifyEnd());

        graph = optimizer.getStructure();
        LaTEXDumper::dumpTreeBlock(output, graph);
        graph->dumpGraph("parsedSimplified.svg");

        LaTEXDumper::rawWrite(output, LaTEXPhrases::getPrimaryDiff());

        auto *derived = Differentiator::derivative(graph, 'x', output);
        derived->dumpGraph("derivative.svg");
        LaTEXDumper::rawWrite(output, LaTEXPhrases::getPrimaryDiffEnd());
        LaTEXDumper::dumpDiffResult(output, content, derived);
        optimizer.cTor(derived);

        LaTEXDumper::rawWrite(output, LaTEXPhrases::getSecondSimplify());
        optimizer.simplify(false, output);
        derived = optimizer.getStructure();
        derived->dumpGraph("derivativeSimplified.svg");
        LaTEXDumper::rawWrite(output, LaTEXPhrases::getSecondSimplifyEnd());
        LaTEXDumper::dumpDiffResult(output, content, derived);

        LaTEXDumper::rawWrite(output, LaTEXPhrases::getFullDerivative());

        char *vars = expressionVariables(tokens->getStorage(), tokens->getSize());

        size_t varNum = 0;
        BinaryTree<ExprNode> **partialDerivatives = derivativeFull(initialGraph, vars, varNum, output);

        dumpFullDerivative(output, vars, partialDerivatives);

        LaTEXDumper::rawWrite(output, LaTEXPhrases::getFullDerivativeSimplify());

        BinaryTree<ExprNode> **partialDerivativesSimplified = fullDerivativeSimplify(partialDerivatives, varNum, output);

        LaTEXDumper::rawWrite(output, LaTEXPhrases::getFullDerivativeResult());
        dumpFullDerivative(output, vars, partialDerivativesSimplified);

        free(partialDerivativesSimplified);
        free(partialDerivatives);

        ClassicStack<ExprNode>::Delete(tokens);
        BinaryTree<ExprNode>::Delete(graph);
        BinaryTree<ExprNode>::Delete(derived);
        LaTEXDumper::dumpDocEnd(output);
        LaTEXPhrases::freeSpace();
    }
};

#endif //DIFFERENTIATOR_DIFFERENTIATOR_H
