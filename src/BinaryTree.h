//
// Created by Александр Дремов on 24.11.2020.
//

#ifndef DIFFERENTIATOR_BINARYTREE_H
#define DIFFERENTIATOR_BINARYTREE_H

#include <cstdlib>

template<typename Elem>
class BinaryTree {
    Elem        value;
    BinaryTree* left;
    BinaryTree* right;
public:
    static BinaryTree* New() {
        auto* thou  = static_cast<BinaryTree*>(calloc(1, sizeof(BinaryTree)));
        thou->left  = nullptr;
        thou->right = nullptr;
        return thou;
    }

    void cTor(Elem newValue) {
        this->value  = newValue;
        this->left   = nullptr;
        this->right  = nullptr;
    }

    void cTor(Elem newValue, BinaryTree* newLeft, BinaryTree* newRight) {
        this->value  = newValue;
        this->left   = newLeft;
        this->right  = newRight;
    }

    void dTor(){
        if (left){
            Delete(left);
        }
        if (right)
            Delete(right);
    }

    static void Delete(BinaryTree* obj) {
        obj->dTor();
        free(obj);
    }

    const Elem& getVal() const{
        return value;
    }

    BinaryTree* getLeft() const{
        return left;
    }

    BinaryTree* getRight() const{
        return right;
    }

    BinaryTree* copy() const{
        BinaryTree* newCopy = New();
        newCopy->cTor(value, left, right);
        return newCopy;
    }

    BinaryTree* deepCopy() const {
        if (left == nullptr && right == nullptr)
            return this->copy();
        BinaryTree* newCopy = New();
        newCopy->cTor(value, left? left->deepCopy(): nullptr, right? right->deepCopy(): nullptr);
        return newCopy;
    }
};

#endif //DIFFERENTIATOR_BINARYTREE_H
