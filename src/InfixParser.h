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
#include "ExprNode.h"
#include "Stack.h"

class InfixParser {
    ClassicStack<ExprNode> stack;

    static ExprNode fetchToken(char*& input) {
        ExprNode retValue{};
        while ((*input == ' ' || *input == '\n') && *input != '\0') {input++;}
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
                return ( retValue.cTor(*input), input += 1, retValue);
            }
        }
    }

public:
    static InfixParser* New() {
        auto* thou  = static_cast<InfixParser*>(calloc(1, sizeof(InfixParser)));
        thou->cTor();
        return thou;
    }

    static ClassicStack<ExprNode> tokenize(char* input) {
        ClassicStack<ExprNode> tokens{};
        tokens.cTor(10);
        while(true) {
            ExprNode newToken = fetchToken(input);
            if (*input == '\0')
                break;
            tokens.push(newToken);
        }
        return tokens;
    }

    BinaryTree<ExprNode>* parseExpression(char* input){
        ClassicStack<ExprNode> entities = tokenize(input);

        ClassicStack<ExprNode> operatorStack{};
        operatorStack.cTor(10);

        ClassicStack<ExprNode> outputStack{};
        operatorStack.cTor(10);



        auto* tree = BinaryTree<ExprNode>::New();
        return tree;
    }

    void cTor() {
        this->stack.cTor(10);
    }

    void dTor(){
        this->stack.dTor();
    }

    static void Delete(InfixParser* obj) {
        obj->dTor();
        free(obj);
    }
};

#endif //DIFFERENTIATOR_INFIXPARSER_H
