//
// Created by Александр Дремов on 24.11.2020.
//

#ifndef DIFFERENTIATOR_BINARYTREE_H
#define DIFFERENTIATOR_BINARYTREE_H

#include <cstdlib>
#include "ExprNode.h"

template<typename Elem>
class BinaryTree {
    Elem value;
    BinaryTree *left;
    BinaryTree *right;

    void dumpRecursive(FILE *output) const {
        switch (value.getType()) {
            case TP_CST: {
                fprintf(output, "node%p[label=\"%lg\" fillcolor=lightgreen style=filled", this, value.getConst());
                break;
            }
            case TP_OPR: {
                fprintf(output, "node%p[label=\"%s\" shape=invhouse fillcolor=aliceblue style=filled",
                        this, value.getOperator().toString());
                break;
            }
            case TP_VAR: {
                fprintf(output, "node%p[label=\"%c\" fillcolor=pink style=filled", this, value.getVar());
                break;
            }
        }
        fprintf(output, "]\n");
        if (left) {
            fprintf(output, "node%p->node%p\n", this, left);
            left->dumpRecursive(output);
        }
        if (right) {
            fprintf(output, "node%p->node%p\n", this, right);
            right->dumpRecursive(output);
        }
    }

public:
    static BinaryTree *New() {
        auto *thou = static_cast<BinaryTree *>(calloc(1, sizeof(BinaryTree)));
        thou->left = nullptr;
        thou->right = nullptr;
        return thou;
    }

    void cTor(Elem newValue) {
        this->value = newValue;
        this->left = nullptr;
        this->right = nullptr;
    }

    void cTor(Elem newValue, BinaryTree *newLeft, BinaryTree *newRight) {
        this->value = newValue;
        this->left = newLeft;
        this->right = newRight;
    }

    void dTor() {
        if (left) {
            Delete(left);
        }
        if (right)
            Delete(right);
    }

    static void Delete(BinaryTree *obj) {
        if (!obj)
            return;
        obj->dTor();
        free(obj);
    }

    const Elem &getVal() const {
        return value;
    }

    BinaryTree *getLeft() const {
        return left;
    }

    BinaryTree *getRight() const {
        return right;
    }

    BinaryTree *&getRight() {
        return right;
    }

    BinaryTree *&getLeft() {
        return left;
    }

    void setRight(BinaryTree *newRight) {
        right = newRight;
    }

    void setLeft(BinaryTree *newLeft) {
        left = newLeft;
    }

    void setVal(Elem newValue) {
        value = newValue;
    }

    BinaryTree *copy() const {
        BinaryTree *newCopy = New();
        newCopy->cTor(value, left, right);
        return newCopy;
    }

    BinaryTree *deepCopy() const {
        if (left == nullptr && right == nullptr)
            return this->copy();
        BinaryTree *newCopy = New();
        newCopy->cTor(value, left ? left->deepCopy() : nullptr, right ? right->deepCopy() : nullptr);
        return newCopy;
    }

    void dumpGraph(const char *fileOut = "output/graph.svg") const {
        const int MAX_LEN = 255;
        FILE *tmpGr = fopen("output/graph.gv", "w");
        fprintf(tmpGr, "digraph expr{\n");
        this->dumpRecursive(tmpGr);
        fprintf(tmpGr, "}\n");
        fclose(tmpGr);

        char command[MAX_LEN] = {};
        sprintf(command, "dot -Tsvg output/graph.gv -o %s", fileOut);
        system(command);
    }

    bool deepCompare(const BinaryTree *other) {
        if (!other)
            return false;
        if (other->getVal() != value)
            return false;
        bool res = true;
        if (left)
            res &= left->deepCompare(other->left);
        else {
            if (other->left)
                return false;
        }
        if (right)
            res &= right->deepCompare(other->right);
        else {
            if (other->right)
                return false;
        }
        return res;
    }
};

#endif //DIFFERENTIATOR_BINARYTREE_H
