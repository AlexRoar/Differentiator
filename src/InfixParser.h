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

class InfixParser {
    ClassicStack<ExprNode> stack;

    static ExprNode fetchToken(char *&input) {
        ExprNode retValue {};
        while ((*input == ' ' || *input == '\n') && *input != '\0') { input++; }
        if (*input == '\0')
            return retValue;
        switch (*input) {
            case '/':
                return (input++, retValue.cTor(OP_DIV), retValue);
            case '*':
                return (input++, retValue.cTor(OP_MUL), retValue);
            case '+':
                return (input++, retValue.cTor(OP_ADD), retValue);
            case '-':
                return (input++, retValue.cTor(OP_SUB), retValue);
            case '^':
                return (input++, retValue.cTor(OP_EXP), retValue);
            case '(':
                return (input++, retValue.cTor(OP_LPA), retValue);
            case ')':
                return (input++, retValue.cTor(OP_RPA), retValue);
            default: {
                int offset = 0;
                double constant = 0;
                if (sscanf(input, "%lg%n", &constant, &offset) > 0) {
                    input += offset;
                    return (retValue.cTor(constant), retValue);
                } else if (strstr(input, "sin") == input)
                    return (input += 3, retValue.cTor(OP_SIN), retValue);
                else if (strstr(input, "cos") == input)
                    return (input += 3, retValue.cTor(OP_COS), retValue);
                else if (strstr(input, "ctg") == input)
                    return (input += 3, retValue.cTor(OP_CTG), retValue);
                else if (strstr(input, "log") == input)
                    return (input += 3, retValue.cTor(OP_LOG), retValue);
                else if (strstr(input, "tan") == input)
                    return (input += 3, retValue.cTor(OP_TAN), retValue);
                return (retValue.cTor(*input), input += 1, retValue);
            }
        }
    }

public:
    static InfixParser *New() {
        auto *thou = static_cast<InfixParser *>(calloc(1, sizeof(InfixParser)));
        thou->cTor();
        return thou;
    }

    static ClassicStack<ExprNode> tokenize(char *input) {
        ClassicStack<ExprNode> tokens {};
        tokens.cTor(10);
        while (true) {
            ExprNode newToken = fetchToken(input);
            tokens.push(newToken);
            if (*input == '\0')
                break;
        }
        return tokens;
    }

    static BinaryTree<ExprNode> *parseExpression(char *input) {
        ClassicStack<ExprNode> entities = tokenize(input);
        auto outputList = SwiftyList<ExprNode>(10, 0, nullptr, false);

        ClassicStack<ExprNode> operatorStack {};
        operatorStack.cTor(10);

        auto *tokensStorage = entities.getStorage();

        for (size_t i = 0; i < entities.getSize(); i++) {
            ExprNode currentToken = tokensStorage[i];
            switch (currentToken.getType()) {
                case TP_CST:
                case TP_VAR:
                    outputList.pushBack(currentToken);
                    break;
                case TP_OPR: {
                    if (currentToken.getOperator().getOpType() == OP_T_FUNC){
                        operatorStack.push(currentToken);
                        break;
                    } else if (currentToken.getOperator().getCode() == OP_LPA) {
                        operatorStack.push(currentToken);
                    } else if (currentToken.getOperator().getCode() == OP_RPA) {
                        if (operatorStack.getSize() == 0)
                            return nullptr;
                        while (operatorStack.top().getOperator().getCode() != OP_LPA) {
                            ExprNode popped = operatorStack.pop();
                            outputList.pushBack(popped);
                            if (operatorStack.getSize() == 0)
                                return nullptr;
                        }
                        operatorStack.pop();
                        if (operatorStack.top().getOperator().getOpType() == OP_T_FUNC){
                            ExprNode popped = operatorStack.pop();
                            outputList.pushBack(popped);
                        }
                    } else {
                        if (operatorStack.getSize() != 0) {
                            while (operatorStack.top().getOperator().getCode() != OP_LPA && operatorStack.top().getOperator() >
                                   currentToken.getOperator() ||
                                   (operatorStack.top().getOperator().getPrecedence() ==
                                    currentToken.getOperator().getPrecedence() && currentToken.getOperator().getAssoc() == ASSOC_LEFT)) {
                                ExprNode popped = operatorStack.pop();
                                outputList.pushBack(popped);
                                if (operatorStack.getSize() == 0)
                                    break;
                            }
                        }
                        operatorStack.push(currentToken);
                    }
                }
            }
        }

        while (operatorStack.getSize() != 0) {
            ExprNode popped = operatorStack.pop();
            outputList.pushBack(popped);
        }
        operatorStack.dTor();
        entities.dTor();

        ClassicStack<BinaryTree<ExprNode>* > converter {};
        converter.cTor(10);

        for (size_t it = outputList.begin(); it != 0; outputList.nextIterator(&it)) {
            ExprNode token {};
            outputList.get(it, &token);
            auto* newNode =  BinaryTree<ExprNode>::New();
            if (token.getType() != TP_OPR) {
                newNode->cTor(token);
            } else {
                BinaryTree<ExprNode>* right = converter.pop();
                BinaryTree<ExprNode>* left = nullptr;
                if (token.getOperator().getArgsCount() == 2) {
                    left = converter.pop();
                }
                newNode->cTor(token, left, right);
            }
            converter.push(newNode);
        }
        
        BinaryTree<ExprNode>* root = converter.pop();
        converter.dTor();
        return root;
    }

    void cTor() {
        this->stack.cTor(10);
    }

    void dTor() {
        this->stack.dTor();
    }

    static void Delete(InfixParser *obj) {
        obj->dTor();
        free(obj);
    }
};

#endif //DIFFERENTIATOR_INFIXPARSER_H
