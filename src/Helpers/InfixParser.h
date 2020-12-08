//
// Created by Александр Дремов on 24.11.2020.
//

#ifndef DIFFERENTIATOR_INFIXPARSER_H
#define DIFFERENTIATOR_INFIXPARSER_H

#include <cctype>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "BinaryTree.h"
#include "SwiftyList/SwiftyList.hpp"
#include "ExprNode.h"
#include "Stack.h"

#define SIMPLE_OP(OP) (input++, retValue.cTor(OP), retValue)
#define SIMPLE_FUNC(FUNC, OP) if (strstr(input, #FUNC) == input)                                       \
                                   return (input += sizeof(#FUNC) - 1, retValue.cTor(OP), retValue)

namespace InfixParser {
    ExprNode fetchToken(char *&input) {
        ExprNode retValue {};

        // Skip invisible characters
        while ((*input == ' ' || *input == '\n' || isspace(*input)) && *input != '\0') { input++; }
        if (*input == '\0')
            return retValue;
        switch (*input) {
            case '/':
                return SIMPLE_OP(OP_DIV);
            case '*':
                return SIMPLE_OP(OP_MUL);
            case '+':
                return SIMPLE_OP(OP_ADD);
            case '^':
                return SIMPLE_OP(OP_POW);
            case '(':
                return SIMPLE_OP(OP_LPA);
            case ')':
                return SIMPLE_OP(OP_RPA);
            case '-': {
                int offset = 0;
                double constant = 0;
                if (sscanf(input, "%lg%n", &constant, &offset) > 0) {
                    input += offset;
                    return (retValue.cTor(constant), retValue);
                }
                return SIMPLE_OP(OP_SUB);
            }
            default: {
                int offset = 0;
                double constant = 0;
                if (sscanf(input, "%lg%n", &constant, &offset) > 0) {
                    input += offset;
                    return (retValue.cTor(constant), retValue);
                }
#define DEF_FUNC(OP_CODE, string, latex, eval, derivative) else SIMPLE_FUNC(string, OP_CODE);
#include "Syntax/Syntax.h"
#undef DEF_FUNC
                return (retValue.cTor(*input), input++, retValue);
            }
        }
    }

    ClassicStack<ExprNode>* tokenize(char *input) {
        const size_t initialStackLen = 10;
        auto* tokens = ClassicStack<ExprNode>::New();
        tokens->cTor(initialStackLen);
        while (true) {
            ExprNode newToken = fetchToken(input);
            tokens->push(newToken);
            if (*input == '\0')
                break;
        }
        return tokens;
    }

    ClassicStack<BinaryTree<ExprNode> *> convertStack(SwiftyList<ExprNode> &outputList);

/**
     * @brief Shunting-yard algorithm
     * https://en.wikipedia.org/wiki/Shunting-yard_algorithm
     *
     * @param input - input char line
     * @param tokenizeRes - tokenize result
     * @return expression binary tree
     */
    BinaryTree<ExprNode> *parseExpression(char *input, ClassicStack<ExprNode>**tokenizeRes=nullptr) {
        ClassicStack<ExprNode>* entities = tokenize(input);
        auto outputList = SwiftyList<ExprNode>(10, 0, nullptr, false);

        if (tokenizeRes) {
            *tokenizeRes = entities;
        }

        ClassicStack<ExprNode> operatorStack {}; operatorStack.cTor(10);

        auto *tokensStorage = entities->getStorage();
        for (size_t i = 0; i < entities->getSize(); i++) {
            ExprNode currentToken = tokensStorage[i];
            switch (currentToken.getType()) {
                case TP_CST:
                case TP_VAR:
                    // if var or constant => just push to output
                    outputList.pushBack(currentToken);
                    break;
                case TP_OPR: {
                    if (currentToken.getOperator().getOpType() == OP_T_FUNC) {
                        // if function => the highest priority => push to operator stack
                        operatorStack.push(currentToken);
                        break;
                    } else if (currentToken.getOperator().getCode() == OP_LPA) {
                        operatorStack.push(currentToken);
                    } else if (currentToken.getOperator().getCode() == OP_RPA) {
                        // If encountered right parenthesis, pop from operator stack
                        // to output stack until LPA seen
                        if (operatorStack.getSize() == 0)
                            return nullptr;
                        while (operatorStack.top()->getOperator().getCode() != OP_LPA) {
                            ExprNode popped = operatorStack.pop();
                            outputList.pushBack(popped);
                            if (operatorStack.getSize() == 0)
                                return nullptr;
                        }
                        operatorStack.pop();
                        if (!operatorStack.isEmpty()) {
                            // check for function on top => ex: sin ( a )
                            if (operatorStack.top()->getOperator().getOpType() == OP_T_FUNC) {
                                ExprNode popped = operatorStack.pop();
                                outputList.pushBack(popped);
                            }
                        }
                    } else {
                        if (operatorStack.getSize() != 0) {
                            // Pop to output while there is operator with higher priority or with the same priority,
                            // but LEFT associativity
                            while (operatorStack.top()->getOperator().getCode() != OP_LPA &&
                                   operatorStack.top()->getOperator() > currentToken.getOperator() ||
                                   (operatorStack.top()->getOperator().getPrecedence() == currentToken.getOperator().getPrecedence() &&
                                    currentToken.getOperator().getAssoc() == ASSOC_LEFT)) {
                                ExprNode popped = operatorStack.pop();
                                outputList.pushBack(popped);
                                if (operatorStack.getSize() == 0)
                                    break;
                            }
                        }
                        // Finally, push new operator
                        operatorStack.push(currentToken);
                    }
                }
            }
        }

        // pop from operators stack to the output stack
        while (operatorStack.getSize() != 0) {
            ExprNode popped = operatorStack.pop();
            if (popped.getType() == TP_OPR && (popped.getOperator().getCode() == OP_LPA ||
                                               popped.getOperator().getCode() == OP_RPA))
                return nullptr;
            outputList.pushBack(popped);
        }
        operatorStack.dTor();
        if (!tokenizeRes) {
            ClassicStack<ExprNode>::Delete(entities);
        }

        ClassicStack<BinaryTree<ExprNode> *> converter = convertStack(outputList);
        BinaryTree<ExprNode> *root = converter.pop();

        if (!converter.isEmpty())
            return nullptr;

        converter.dTor();
        outputList.DestructList();
        return root;
    }

    ClassicStack<BinaryTree<ExprNode> *> convertStack(SwiftyList<ExprNode> &outputList) {
        ClassicStack<BinaryTree<ExprNode> *> converter {}; converter.cTor(10);

        for (size_t it = outputList.begin(); it != 0; outputList.nextIterator(&it)) {
            ExprNode token {};
            outputList.get(it, &token);
            auto *newNode = BinaryTree<ExprNode>::New();
            if (token.getType() != TP_OPR) {
                newNode->cTor(token);
            } else {
                BinaryTree<ExprNode> *right = converter.pop();
                BinaryTree<ExprNode> *left = nullptr;
                if (token.getOperator().getArgsCount() == 2) {
                    left = converter.pop();
                }
                newNode->cTor(token, left, right);
            }
            converter.push(newNode);
        }
        return converter;
    }
};

#endif //DIFFERENTIATOR_INFIXPARSER_H
