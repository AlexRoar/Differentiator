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
            case OP_EXP: {
                EvaluatorRes rightVal = Evaluator::eval(head->getRight(), true);
                if (rightVal.res == 0.5 && rightVal.status == EV_OK) {
                    fprintf(output, "\\sqrt{");
                    dumpTree(output, head->getLeft(), EXPECT_OPERATOR_LOWER(head->getLeft(), OP_EXP));
                    fprintf(output, "}");
                    if (paranthesis)
                        fprintf(output, "\\right)");
                } else {
                    fprintf(output, "{");
                    dumpTree(output, head->getLeft(), EXPECT_OPERATOR_LOWER(head->getLeft(), OP_EXP));
                    fprintf(output, "}^{");
                    dumpTree(output, head->getRight(), EXPECT_OPERATOR_LOWER(head->getRight(), OP_EXP));
                    fprintf(output, "}");
                }
                break;
            }
            case OP_SIN: {
                fprintf(output, "\\sin{");
                dumpTree(output, head->getRight(), EXPECT_OPERATOR_LOWER_OR_EQ(head->getRight(), OP_SIN));
                fprintf(output, "}");
                break;
            }
            case OP_COS: {
                fprintf(output, "\\cos{");
                dumpTree(output, head->getRight(), EXPECT_OPERATOR_LOWER_OR_EQ(head->getRight(), OP_COS));
                fprintf(output, "}");
                break;
            }
            case OP_TAN: {
                fprintf(output, "\\tan{");
                dumpTree(output, head->getRight(), EXPECT_OPERATOR_LOWER_OR_EQ(head->getRight(), OP_TAN));
                fprintf(output, "}");
                break;
            }
            case OP_CTG: {
                fprintf(output, "\\cot{");
                dumpTree(output, head->getRight(), EXPECT_OPERATOR_LOWER_OR_EQ(head->getRight(), OP_CTG));
                fprintf(output, "}");
                break;
            }
            case OP_LOG: {
                fprintf(output, "\\log{");
                dumpTree(output, head->getRight(), EXPECT_OPERATOR_LOWER_OR_EQ(head->getRight(), OP_LOG));
                fprintf(output, "}");
                break;
            }
            case OP_ATAN: {
                fprintf(output, "\\arctan{");
                dumpTree(output, head->getRight(), EXPECT_OPERATOR_LOWER_OR_EQ(head->getRight(), OP_ATAN));
                fprintf(output, "}");
                break;
            }
            case OP_ACOS: {
                fprintf(output, "\\arccos{");
                dumpTree(output, head->getRight(), EXPECT_OPERATOR_LOWER_OR_EQ(head->getRight(), OP_ACOS));
                fprintf(output, "}");
                break;
            }
            case OP_ASIN: {
                fprintf(output, "\\arcsin{");
                dumpTree(output, head->getRight(), EXPECT_OPERATOR_LOWER_OR_EQ(head->getRight(), OP_ASIN));
                fprintf(output, "}");
                break;
            }
            case OP_ACTG: {
                fprintf(output, "\\arccot{");
                dumpTree(output, head->getRight(), EXPECT_OPERATOR_LOWER_OR_EQ(head->getRight(), OP_ACTG));
                fprintf(output, "}");
                break;
            }
            default: {
                printf("Reached prohibited operation in %s: %s\n", __FILE__, __PRETTY_FUNCTION__);
                return;
            }
        }
        if (paranthesis)
            fprintf(output, "\\right)");
    }

    void dumpDocStart(FILE *output) {
        fprintf(output, "\\documentclass[11pt, oneside]{article}\n"
                        "\\usepackage{amsmath,amsthm,amssymb}\n"
                        "\\usepackage{mathtext}\n"
                        "\\usepackage[T1,T2A]{fontenc}\n"
                        "\\usepackage[utf8]{inputenc}\n"
                        "\\usepackage[english,bulgarian,ukrainian,russian]{babel}\n"
                        "\\usepackage{algorithm2e}\n"
                        "\\usepackage{indentfirst}\n"
                        "\\setlength{\\parindent}{15pt}"
                        "\\usepackage{mathtools}"
                        "\\usepackage[a4paper, total={6in, 10in}]{geometry}\n"
                        "\\newcommand\\ddfrac[2]{\\frac{\\displaystyle #1}{\\displaystyle #2}}"
                        "\\begin{document}");
        fprintf(output, "%s", LaTEXPhrases::docStart);
        fprintf(output, "%s", LaTEXPhrases::intro);
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
