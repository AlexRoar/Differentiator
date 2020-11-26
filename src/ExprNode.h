//
// Created by Александр Дремов on 24.11.2020.
//

#ifndef DIFFERENTIATOR_EXPRTYPE_H
#define DIFFERENTIATOR_EXPRTYPE_H

#include <cstdlib>

enum ExprType {
    TP_VAR  = 'x',
    TP_CST  = 'a',
    TP_OPR  = 'o'
};

enum ExprOperatorCode {
    OP_SUB  = 100  + '-',
    OP_ADD  = 200  + '+',
    OP_DIV  = 300  + '/',
    OP_MUL  = 400  + '*',
    OP_EXP  = 500  + '^',
    OP_SIN  = 600  + 's',
    OP_COS  = 700  + 'c',
    OP_TAN  = 800  + 't',
    OP_CTG  = 900  + 'g',
    OP_LOG  = 1000 + 'l',
    OP_LPA  = 1100 + '(',
    OP_RPA  = 1200 + ')',
    OP_ATAN = 1300 + 't',
    OP_ACOS = 1400 + 'c',
    OP_ASIN = 1500 + 's',
    OP_ACTG = 1600 + 'c',
};

enum ExprOperatorType {
    OP_T_OPER = 100,
    OP_T_FUNC = 200
};

enum ExprAssociativity {
    ASSOC_LEFT  = -10,
    ASSOC_RIGHT =  10,
    ASSOC_NONE  =  0
};

class ExprOperator {
    ExprOperatorCode   code;
    ExprAssociativity  assoc;
    ExprOperatorType   opType;
    unsigned           precedence;
    unsigned           argsCount;
public:
    static ExprOperator* New() {
        auto* thou  = static_cast<ExprOperator*>(calloc(1, sizeof(ExprOperator)));
        thou->cTor(OP_ADD);
        return thou;
    }

    void cTor(ExprOperatorCode newCode) {
        this->code = newCode;
        this->argsCount = 0;
        this->precedence = 0;
        this->assoc = ASSOC_NONE;
        this->updateParameters();
    }

    void updateParameters() {
        switch (code) {
            case OP_SUB:
            case OP_ADD: {
                assoc      = ASSOC_LEFT;
                precedence = 2;
                argsCount  = 2;
                opType     = OP_T_OPER;
                break;
            }
            case OP_MUL:
            case OP_DIV: {
                assoc      = ASSOC_LEFT;
                precedence = 3;
                argsCount  = 2;
                opType     = OP_T_OPER;
                break;
            }
            case OP_EXP: {
                assoc      = ASSOC_RIGHT;
                precedence = 4;
                argsCount  = 2;
                opType     = OP_T_OPER;
                break;
            }
            case OP_SIN:
            case OP_COS:
            case OP_TAN:
            case OP_CTG:
            case OP_ATAN:
            case OP_ACOS:
            case OP_ASIN:
            case OP_ACTG:
            case OP_LOG: {
                assoc      = ASSOC_NONE;
                precedence = 5;
                argsCount  = 1;
                opType     = OP_T_FUNC;
                break;
            }
            case OP_RPA:
            case OP_LPA:{
                assoc      = ASSOC_NONE;
                precedence = 6;
                opType     = OP_T_OPER;
                break;
            }
        }
    }

    void dTor(){}

    static void Delete(ExprOperator* obj) {
        obj->dTor();
        free(obj);
    }

    [[nodiscard]] ExprAssociativity getAssoc() const{
        return assoc;
    }

    [[nodiscard]] ExprOperatorCode getCode() const{
        return code;
    }

    [[nodiscard]] unsigned getPrecedence() const{
        return precedence;
    }

    [[nodiscard]] unsigned getArgsCount() const{
        return argsCount;
    }

    [[nodiscard]] ExprOperatorType getOpType() const{
        return opType;
    }

    const char* toString() {
        switch (code) {
            case OP_SUB:  return "-";
            case OP_ADD:  return "+";
            case OP_MUL:  return "*";
            case OP_DIV:  return "/";
            case OP_EXP:  return "^";
            case OP_SIN:  return "sin";
            case OP_COS:  return "cos";
            case OP_TAN:  return "tan";
            case OP_CTG:  return "ctg";
            case OP_LOG:  return "log";
            case OP_RPA:  return "(";
            case OP_LPA:  return ")";
            case OP_ATAN: return "arctan";
            case OP_ACOS: return "arccos";
            case OP_ASIN: return "arcsin";
            case OP_ACTG: return "arccot";
        }
    }

    bool operator<(const ExprOperator& other) const {
        return precedence < other.precedence;
    }

    bool operator<=(const ExprOperator& other) const {
        return precedence <= other.precedence;
    }

    bool operator>(const ExprOperator& other) const {
        return precedence > other.precedence;
    }

    bool operator>=(const ExprOperator& other) const {
        return precedence >= other.precedence;
    }

    bool operator==(const ExprOperator& other) const {
        return code == other.code;
    }
};

class ExprNode {
    union {
        char            variable;
        double          value;
        ExprOperator    op;
    };
    ExprType        type;

public:
    static ExprNode* New() {
        auto* thou  = static_cast<ExprNode*>(calloc(1, sizeof(ExprNode)));
        return thou;
    }

    void cTor(char newVariable) {
        this->variable = newVariable;
        this->type     = TP_VAR;
    }

    void cTor(double newValue) {
        this->value = newValue;
        this->type  = TP_CST;
    }

    void cTor(ExprOperatorCode exprOperatorCode) {
        this->op = ExprOperator();
        op.cTor(exprOperatorCode);
        this->type  = TP_OPR;
    }

    void dTor(){}

    static void Delete(ExprNode* obj) {
        obj->dTor();
        free(obj);
    }

    [[nodiscard]] ExprType getType() const{
        return type;
    }

    [[nodiscard]] char getVar() const{
        return variable;
    }

    [[nodiscard]] double getConst() const{
        return value;
    }

    [[nodiscard]] ExprOperator getOperator() const{
        return op;
    }

    bool operator==(const ExprNode& other){
        if (type != other.type)
            return false;
        switch (type) {
            case TP_VAR:
                return variable == other.variable;
            case TP_CST:
                return value == other.value;
            case TP_OPR:
                return op == other.op;
        }
    }
};

#endif //DIFFERENTIATOR_EXPRTYPE_H
