//
// Created by Александр Дремов on 25.11.2020.
//

#ifndef DIFFERENTIATOR_LATEXDUMPER_H
#define DIFFERENTIATOR_LATEXDUMPER_H

#include <cstdio>
#include <Helpers/BinaryTree.h>
#include <ExprNode.h>
#include <LaTEX/LaTEXPhrases.h>
#include <Evaluation/EvaluatorRes.h>
#include <Evaluation/Evaluator.h>

#define EXPECT_OPERATOR_LOWER(node, op) (node->getVal().getType() == TP_OPR) && \
                                        (node->getVal().getOperator() < op)  || \
                                        (node->getVal().getType() != TP_OPR)

#define EXPECT_OPERATOR_LOWER_OR_EQ(node, op) (node->getVal().getType() == TP_OPR) && \
                                              (node->getVal().getOperator() <= op)  || \
                                              (node->getVal().getType() != TP_OPR)
namespace LaTEXDumper {
    void dumpTree(FILE *output, BinaryTree<ExprNode> *head, bool paranthesis = true) {
        if (!head)
            return;
        if (head->getVal().getType() == TP_VAR) {
            fprintf(output, "%c", head->getVal().getVar());
            return;
        } else if (head->getVal().getType() == TP_CST) {
            fprintf(output, "%.20g", head->getVal().getConst());
            return;
        }
        if (paranthesis)
            fprintf(output, "\\left(");

        switch (head->getVal().getOperator().getCode()) {
            case OP_SUB: {
                dumpTree(output, head->getLeft(), EXPECT_OPERATOR_LOWER(head->getRight(), OP_SUB));
                fprintf(output, " - ");
                dumpTree(output, head->getRight(), true);
                break;
            }
            case OP_ADD: {
                dumpTree(output, head->getLeft(), EXPECT_OPERATOR_LOWER(head->getLeft(), OP_ADD));
                fprintf(output, " + ");
                dumpTree(output, head->getRight(), EXPECT_OPERATOR_LOWER(head->getRight(), OP_ADD));
                break;
            }
            case OP_DIV: {
                fprintf(output, "\\frac{");
                dumpTree(output, head->getLeft(), EXPECT_OPERATOR_LOWER(head->getLeft(), OP_DIV));
                fprintf(output, "}{");
                dumpTree(output, head->getRight(), EXPECT_OPERATOR_LOWER(head->getRight(), OP_DIV));
                fprintf(output, "}");
                break;
            }
            case OP_MUL: {
                if (head->getLeft()->getVal().getType() == TP_VAR) {
                    dumpTree(output, head->getRight(), EXPECT_OPERATOR_LOWER(head->getRight(), OP_MUL));
                    fprintf(output, " \\cdot ");
                    dumpTree(output, head->getLeft(), EXPECT_OPERATOR_LOWER(head->getLeft(), OP_MUL));
                } else {
                    dumpTree(output, head->getLeft(), EXPECT_OPERATOR_LOWER(head->getLeft(), OP_MUL));
                    fprintf(output, " \\cdot ");
                    dumpTree(output, head->getRight(), EXPECT_OPERATOR_LOWER(head->getRight(), OP_MUL));
                }
                break;
            }
            case OP_POW: {
                EvaluatorRes rightVal = Evaluator::eval(head->getRight(), true);
                if (rightVal.res == 0.5 && rightVal.status == EV_OK) {
                    fprintf(output, "\\sqrt{");
                    dumpTree(output, head->getLeft(), false);
                    fprintf(output, "}");
                } else {
                    fprintf(output, "{");
                    fprintf(output, "\\left(");
                    dumpTree(output, head->getLeft(), EXPECT_OPERATOR_LOWER_OR_EQ(head->getLeft(), OP_POW));
                    fprintf(output, "\\right)");
                    fprintf(output, "}^{");
                    dumpTree(output, head->getRight(), EXPECT_OPERATOR_LOWER_OR_EQ(head->getRight(), OP_POW));
                    fprintf(output, "}");
                }
                break;
            }
#define DEF_FUNC(OP_CODE, string, latex, eval, derivative) case OP_CODE: {                                   \
                fprintf(output, "\\" #latex "{");                                                             \
                dumpTree(output, head->getRight(), EXPECT_OPERATOR_LOWER_OR_EQ(head->getRight(), OP_CODE));   \
                fprintf(output, "}");                                                                         \
                break;                                                                                        \
}
#include <Syntax/Syntax.h>
#undef DEF_FUNC
            default: {
                printf("Reached prohibited operation in %s: %s\n", __FILE__, __PRETTY_FUNCTION__);
                return;
            }
        }
        if (paranthesis)
            fprintf(output, "\\right)");
    }

    void dumpDocStart(FILE *output) {
        fprintf(output, "%s", LaTEXPhrases::getDocStart());
        fprintf(output, "%s", LaTEXPhrases::getIntro());
    }

    void dumpDocEnd(FILE *output) {
        fprintf(output, "\\end{document}");
    }

    void dumpTreeInline(FILE *output, BinaryTree<ExprNode> *head) {
        if (!output)
            return;
        fprintf(output, "\\(");
        dumpTree(output, head, false);
        fprintf(output, "\\)");
    }

    void dumpTreeBlock(FILE *output, BinaryTree<ExprNode> *head) {
        if (!output)
            return;
        fprintf(output, "\\[");
        dumpTree(output, head, false);
        fprintf(output, "\\]");
    }

    void dumpPartial(FILE *output, BinaryTree<ExprNode> *head, char var) {
        if (!output)
            return;
        dumpTree(output, head, true);
        fprintf(output, " d%c", var);
    }

    void dumpDiffResult(FILE *output, char *input, BinaryTree<ExprNode> *head, char var = 'x') {
        if (!output)
            return;
        fprintf(output, "\\[");
        dumpTree(output, head, false);
        fprintf(output, "\\]");
    }

    void dumpFullLaTEX(FILE *output, BinaryTree<ExprNode> *head) {
        if (!output)
            return;
        dumpDocStart(output);
        fprintf(output, "\\[");
        dumpTree(output, head, false);
        fprintf(output, "\\]");
        dumpDocEnd(output);
    }

    void rawWrite(FILE *output, const char *content) {
        if (output)
            fprintf(output, "%s\n", content);
    }
}

#endif //DIFFERENTIATOR_LATEXDUMPER_H
