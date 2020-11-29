//
// Created by Александр Дремов on 25.11.2020.
//

#ifndef DIFFERENTIATOR_SPECIFICOPTIMISERS_H
#define DIFFERENTIATOR_SPECIFICOPTIMISERS_H

#include "ExprNode.h"
#include "Helpers/BinaryTree.h"
#include "Evaluation/EvaluatorRes.h"
#include "Evaluation/Evaluator.h"

#define EXPECT_EVAL_R_EQ(val) (evalR.status == EV_OK && evalR.res == val)
#define EXPECT_EVAL_L_EQ(val) (evalL.status == EV_OK && evalL.res == val)

#define EXPECT_OP(node, OP) (((node) ->getVal().getType() == TP_OPR) && ((node) ->getVal().getOperator().getCode() == OP))

namespace SpecificOptimizers {
    static unsigned
    mulOptimizer(BinaryTree<ExprNode> *&head, EvaluatorRes evalL, EvaluatorRes evalR, bool evalMath = false) {
        unsigned changed = 0;
        if (EXPECT_EVAL_R_EQ(1)) {
            BinaryTree<ExprNode>::Delete(head->getRight());
            head = head->getLeft();
            changed += 1;
        } else if (EXPECT_EVAL_L_EQ(1)) {
            BinaryTree<ExprNode>::Delete(head->getLeft());
            head = head->getRight();
            changed += 1;
        } else if (EXPECT_EVAL_R_EQ(0) || EXPECT_EVAL_L_EQ(0)) {
            ExprNode newVal {};
            newVal.cTor(0.0);
            BinaryTree<ExprNode>::Delete(head->getLeft());
            BinaryTree<ExprNode>::Delete(head->getRight());
            head->cTor(newVal);
            changed += 1;
        }

            /*
             *          (*)
             *         /   \
             *       (*)   (c)
             *      /   \
             *   (...)  (...)
             */
        else if (head->getLeft() && evalR.status == EV_OK) {
            if (EXPECT_OP(head->getLeft(), OP_MUL)) {
                EvaluatorRes evalLL = Evaluator::eval(head->getLeft()->getLeft(), evalMath);
                EvaluatorRes evalLR = Evaluator::eval(head->getLeft()->getRight(), evalMath);

                /*
                 *          (*)
                 *         /   \
                 *       (*)   (c)
                 *      /   \
                 *    (c)  (VAR)
                 */
                if (evalLL.status == EV_OK) {
                    ExprNode headRight {}, headLeftLeft {};
                    headRight.cTor(evalR.res);
                    headLeftLeft.cTor(evalLL.res);
                    BinaryTree<ExprNode>::Delete(head->getRight());
                    BinaryTree<ExprNode>::Delete(head->getLeft()->getLeft());
                    head = head->getLeft();

                    auto *newLeft = BinaryTree<ExprNode>::New();
                    ExprNode headLeftNew {};
                    headLeftNew.cTor(evalR.res * evalLL.res);
                    newLeft->cTor(headLeftNew);
                    head->setLeft(newLeft);
                    changed++;
                } else if (evalLL.status == EV_OK) {
                    /*
                     *          (*)
                     *         /   \
                     *       (*)   (c)
                     *      /   \
                     *   (VAR)  (c)
                     */
                    ExprNode headRight {}, headLeftRight {};
                    headRight.cTor(evalR.res);
                    headLeftRight.cTor(evalLR.res);
                    BinaryTree<ExprNode>::Delete(head->getRight());
                    BinaryTree<ExprNode>::Delete(head->getLeft()->getRight());
                    head = head->getLeft();

                    auto *newRight = BinaryTree<ExprNode>::New();
                    ExprNode headRightNew {};
                    headRightNew.cTor(evalR.res * evalLR.res);
                    newRight->cTor(headRightNew);
                    head->setRight(newRight);
                    changed++;
                }
            }
            /*
             *          (*)
             *         /   \
             *       (c)   (*)
             *            /   \
             *         (...)  (...)
             */
        } else if (head->getRight() && evalL.status == EV_OK) {
            if (EXPECT_OP(head->getRight(), OP_MUL)) {
                EvaluatorRes evalRL = Evaluator::eval(head->getRight()->getLeft(), evalMath);
                EvaluatorRes evalRR = Evaluator::eval(head->getRight()->getRight(), evalMath);

                /*
                *          (*)
                *         /   \
                *       (c)   (*)
                *            /   \
                *          (c)  (VAR)
                */
                if (evalRL.status == EV_OK) {
                    ExprNode headLeft {}, headRightLeft {};
                    headLeft.cTor(evalL.res);
                    headRightLeft.cTor(evalRL.res);
                    BinaryTree<ExprNode>::Delete(head->getLeft());
                    BinaryTree<ExprNode>::Delete(head->getRight()->getLeft());
                    head = head->getRight();

                    auto *newLeft = BinaryTree<ExprNode>::New();
                    ExprNode headLeftNew {};
                    headLeftNew.cTor(evalL.res * evalRL.res);
                    newLeft->cTor(headLeftNew);
                    head->setLeft(newLeft);
                    changed++;
                } else if (evalRL.status == EV_OK) {
                    /*
                    *          (*)
                    *         /   \
                    *       (c)   (*)
                    *            /   \
                    *          (VAR) (c)
                    */
                    ExprNode headLeft {}, headRightRight {};
                    headLeft.cTor(evalL.res);
                    headRightRight.cTor(evalRR.res);
                    BinaryTree<ExprNode>::Delete(head->getLeft());
                    BinaryTree<ExprNode>::Delete(head->getRight()->getRight());
                    head = head->getRight();

                    auto *newRight = BinaryTree<ExprNode>::New();
                    ExprNode headRightNew {};
                    headRightNew.cTor(evalR.res * evalRR.res);
                    newRight->cTor(headRightNew);
                    head->setRight(newRight);
                    changed++;
                }
            }
        }

            /*
             *          (*)             (^)
             *         /   \    ->     /   \
             *       (t)   (t)       (t)   (2)
             */
        else if (head->getLeft()->deepCompare(head->getRight())) {
            BinaryTree<ExprNode>::Delete(head->getRight());
            ExprNode headRightNew {};
            ExprNode headNew {};
            headRightNew.cTor(2.0);
            headNew.cTor(OP_POW);

            auto *newRight = BinaryTree<ExprNode>::New();
            auto *newLeft = head->getLeft();
            newRight->cTor(headRightNew);
            head->cTor(headNew);
            head->setRight(newRight);
            head->setLeft(newLeft);
            changed++;
        }

            /*
            *          (*)              (^)
            *         /   \            /   \
            *       (^)   (t)  ->    (t)   (+)
            *      /   \                  /   \
            *    (t)  (pow)            (pow)  (1)
            */
        else if (EXPECT_OP(head->getLeft(), OP_POW)) {
            if (head->getLeft()->getLeft()->deepCompare(head->getRight())) {
                BinaryTree<ExprNode>::Delete(head->getRight());

                BinaryTree<ExprNode> *nR = BinaryTree<ExprNode>::New();
                ExprNode headRightNew {};
                headRightNew.cTor(OP_ADD);
                nR->cTor(headRightNew);
                nR->setLeft(head->getLeft()->getRight());

                BinaryTree<ExprNode> *nRR = BinaryTree<ExprNode>::New();
                ExprNode headRightRightNew {};
                headRightRightNew.cTor(1.0);
                nRR->cTor(headRightRightNew);

                nR->setRight(nRR);

                BinaryTree<ExprNode> *nL = head->getLeft()->getLeft();

                ExprNode headNew {};
                headNew.cTor(OP_POW);
                head->cTor(headNew);
                head->setRight(nR);
                head->setLeft(nL);
                changed++;
            }
        }
            /*
            *          (*)              (^)
            *         /   \            /   \
            *       (t)   (^)  ->    (t)   (+)
            *            /   \            /   \
            *          (t)  (pow)       (1)   (pow)
            */
        else if (EXPECT_OP(head->getRight(), OP_POW)) {
            if (head->getLeft()->deepCompare(head->getRight()->getLeft())) {
                BinaryTree<ExprNode>::Delete(head->getLeft());

                BinaryTree<ExprNode> *t = head->getRight()->getLeft();
                BinaryTree<ExprNode> *_pow = head->getRight()->getRight();

                ExprNode add {};
                add.cTor(OP_ADD);
                head->getRight()->cTor(add);

                BinaryTree<ExprNode> *unoNode = BinaryTree<ExprNode>::New();
                ExprNode uno {};
                uno.cTor(1.0);
                unoNode->cTor(uno);
                head->getRight()->setLeft(unoNode);
                head->getRight()->setRight(_pow);
                head->setLeft(t);

                ExprNode newExp {};
                newExp.cTor(OP_POW);
                head->setVal(newExp);
                changed++;
            }
        }

            /*
            *            (*)                         (/)
            *          /     \                     /     \
            *       (/)      (/)      ->        (*)      (*)
            *      /   \    /    \             /   \    /    \
            * (nLL) (nLR) (nRL) (nRR)      (nLL) (nRL) (nLR) (nRR)
            */
        else if (head->getRight() && head->getLeft()) {
            if (EXPECT_OP(head->getRight(), OP_DIV) && EXPECT_OP(head->getLeft(), OP_DIV)) {
                ExprNode mul {};
                mul.cTor(OP_MUL);
                ExprNode div {};
                div.cTor(OP_DIV);

                head->getRight()->setVal(mul);
                head->getLeft()->setVal(mul);
                head->setVal(div);

                BinaryTree<ExprNode> *tmpNode = head->getRight()->getLeft();
                head->getRight()->setLeft(head->getLeft()->getRight());
                head->getLeft()->setRight(tmpNode);
                changed++;
            }
        }

        return changed;
    }

    static unsigned
    addOptimizer(BinaryTree<ExprNode> *&head, EvaluatorRes evalL, EvaluatorRes evalR, bool evalMath = false) {
        unsigned changed = 0;
        if (EXPECT_EVAL_R_EQ(0)) {
            BinaryTree<ExprNode>::Delete(head->getRight());
            head = head->getLeft();
            changed += 1;
        } else if (EXPECT_EVAL_L_EQ(0)) {
            BinaryTree<ExprNode>::Delete(head->getLeft());
            head = head->getRight();
            changed += 1;
        }

        /*
         *          (+)
         *         /   \
         *       (+)   (c)
         *      /   \
         *   (...)  (...)
         */
        if (head->getLeft() && evalR.status == EV_OK) {
            if (EXPECT_OP(head->getLeft(), OP_ADD)) {
                EvaluatorRes evalLL = Evaluator::eval(head->getLeft()->getLeft(), evalMath);
                EvaluatorRes evalLR = Evaluator::eval(head->getLeft()->getRight(), evalMath);

                /*
                 *          (+)
                 *         /   \
                 *       (+)   (c)
                 *      /   \
                 *    (c)  (VAR)
                 */
                if (evalLL.status == EV_OK) {
                    ExprNode headRight {}, headLeftLeft {};
                    headRight.cTor(evalR.res);
                    headLeftLeft.cTor(evalLL.res);
                    BinaryTree<ExprNode>::Delete(head->getRight());
                    BinaryTree<ExprNode>::Delete(head->getLeft()->getLeft());
                    head = head->getLeft();

                    auto *newLeft = BinaryTree<ExprNode>::New();
                    ExprNode headLeftNew {};
                    headLeftNew.cTor(evalR.res + evalLL.res);
                    newLeft->cTor(headLeftNew);
                    head->setLeft(newLeft);
                    changed++;
                } else if (evalLL.status == EV_OK) {
                    /*
                     *          (+)
                     *         /   \
                     *       (+)   (c)
                     *      /   \
                     *   (VAR)  (c)
                     */
                    ExprNode headRight {}, headLeftRight {};
                    headRight.cTor(evalR.res);
                    headLeftRight.cTor(evalLR.res);
                    BinaryTree<ExprNode>::Delete(head->getRight());
                    BinaryTree<ExprNode>::Delete(head->getLeft()->getRight());
                    head = head->getLeft();

                    auto *newRight = BinaryTree<ExprNode>::New();
                    ExprNode headRightNew {};
                    headRightNew.cTor(evalR.res + evalLR.res);
                    newRight->cTor(headRightNew);
                    head->setRight(newRight);
                    changed++;
                }
            }
            /*
             *          (+)
             *         /   \
             *       (c)   (+)
             *            /   \
             *         (...)  (...)
             */
        } else if (head->getRight() && evalL.status == EV_OK) {
            if (EXPECT_OP(head->getRight(), OP_ADD)) {
                EvaluatorRes evalRL = Evaluator::eval(head->getRight()->getLeft(), evalMath);
                EvaluatorRes evalRR = Evaluator::eval(head->getRight()->getRight(), evalMath);

                /*
                *          (+)
                *         /   \
                *       (c)   (+)
                *            /   \
                *          (c)  (VAR)
                */
                if (evalRL.status == EV_OK) {
                    ExprNode headLeft {}, headRightLeft {};
                    headLeft.cTor(evalL.res);
                    headRightLeft.cTor(evalRL.res);
                    BinaryTree<ExprNode>::Delete(head->getLeft());
                    BinaryTree<ExprNode>::Delete(head->getRight()->getLeft());
                    head = head->getRight();

                    auto *newLeft = BinaryTree<ExprNode>::New();
                    ExprNode headLeftNew {};
                    headLeftNew.cTor(evalL.res + evalRL.res);
                    newLeft->cTor(headLeftNew);
                    head->setLeft(newLeft);
                    changed++;
                } else if (evalRL.status == EV_OK) {
                    /*
                    *          (+)
                    *         /   \
                    *       (c)   (+)
                    *            /   \
                    *          (VAR) (c)
                    */
                    ExprNode headLeft {}, headRightRight {};
                    headLeft.cTor(evalL.res);
                    headRightRight.cTor(evalRR.res);
                    BinaryTree<ExprNode>::Delete(head->getLeft());
                    BinaryTree<ExprNode>::Delete(head->getRight()->getRight());
                    head = head->getRight();

                    auto *newRight = BinaryTree<ExprNode>::New();
                    ExprNode headRightNew {};
                    headRightNew.cTor(evalR.res + evalRR.res);
                    newRight->cTor(headRightNew);
                    head->setRight(newRight);
                    changed++;
                }
            }
        }

        /*
         *          (+)             (*)
         *         /   \    ->     /   \
         *       (t)   (t)       (t)   (2)
         */
        else if (head->getLeft()) {
            if (head->getLeft()->deepCompare(head->getRight())) {
                BinaryTree<ExprNode>::Delete(head->getRight());
                ExprNode headRightNew {};
                ExprNode headNew {};
                headRightNew.cTor(2.0);
                headNew.cTor(OP_MUL);

                auto *newRight = BinaryTree<ExprNode>::New();
                auto *newLeft = head->getLeft();
                newRight->cTor(headRightNew);
                head->cTor(headNew);
                head->setRight(newRight);
                head->setLeft(newLeft);
                changed++;
            }
        }

        return changed;
    }

    static unsigned
    divOptimizer(BinaryTree<ExprNode> *&head, EvaluatorRes evalL, EvaluatorRes evalR, bool evalMath = false) {
        unsigned changed = 0;
        if (EXPECT_EVAL_R_EQ(1)) {
            BinaryTree<ExprNode>::Delete(head->getRight());
            head = head->getLeft();
            changed += 1;
        }

        /*   (nLL / nLR) / nR -> (nLL * nR) / nLR
         *          (/)                (/)
         *         /   \              /   \
         *       (/)   (nR)   ->    (*)   (nLR)
         *      /   \              /   \
         *   (nLL)  (nLR)       (nLL)  (nR)
         */
        if (head->getLeft()) {
            if (EXPECT_OP(head->getLeft(), OP_DIV)) {
                BinaryTree<ExprNode> *nLR = head->getLeft()->getRight();
                BinaryTree<ExprNode> *nLL = head->getLeft()->getLeft();
                BinaryTree<ExprNode> *nR = head->getRight();

                ExprNode newOp {};
                newOp.cTor(OP_MUL);
                head->getLeft()->cTor(newOp);
                head->getLeft()->setLeft(nLL);
                head->getLeft()->setRight(nR);
                head->setRight(nLR);
                changed++;
            }
        }
        if (head->getRight()) {
            if (EXPECT_OP(head->getRight(), OP_DIV)) {
                /*   nLL / (nLR / nR) -> (nLL * nR) / nLR
                 *      (/)                   (/)
                 *     /   \                 /   \
                 *   (nL)   (/)      ->    (*)   (nRL)
                 *         /   \          /   \
                 *      (nRL)  (nRR)   (nL)  (nRR)
                 */

                BinaryTree<ExprNode> *nL = head->getLeft();
                BinaryTree<ExprNode> *nRL = head->getRight()->getLeft();
                BinaryTree<ExprNode> *nRR = head->getRight()->getRight();

                ExprNode newOp {}, blankNode {};
                newOp.cTor(OP_MUL);
                blankNode.cTor(0.0);
                head->setLeft(head->getRight());
                head->getLeft()->cTor(newOp);
                head->getLeft()->setRight(nRR);
                head->getLeft()->setLeft(nL);
                head->setRight(nRL);
                changed++;
            }
        }

        return changed;
    }

    static unsigned
    subOptimizer(BinaryTree<ExprNode> *&head, EvaluatorRes evalL, EvaluatorRes evalR, bool evalMath = false) {
        unsigned changed = 0;
        if (EXPECT_EVAL_R_EQ(0)) {
            BinaryTree<ExprNode>::Delete(head->getRight());
            head = head->getLeft();
            changed += 1;
        }
        return changed;
    }

    static unsigned
    expOptimizer(BinaryTree<ExprNode> *&head, EvaluatorRes evalL, EvaluatorRes evalR, bool evalMath = false) {
        unsigned changed = 0;
        if (EXPECT_EVAL_R_EQ(1)) {
            BinaryTree<ExprNode>::Delete(head->getRight());
            head = head->getLeft();
            changed += 1;
        } else if (EXPECT_EVAL_R_EQ(0)) {
            ExprNode newVal {};
            newVal.cTor(1.0);
            BinaryTree<ExprNode>::Delete(head->getLeft());
            BinaryTree<ExprNode>::Delete(head->getRight());
            head->cTor(newVal);
            changed += 1;
        }

        return changed;
    }
}
#undef EXPECT_EVAL_R_EQ
#undef EXPECT_EVAL_L_EQ
#endif //DIFFERENTIATOR_SPECIFICOPTIMISERS_H
