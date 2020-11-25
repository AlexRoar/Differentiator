//
// Created by Александр Дремов on 25.11.2020.
//

#ifndef DIFFERENTIATOR_LATEXDUMPER_H
#define DIFFERENTIATOR_LATEXDUMPER_H

#include <cstdio>
#include "Helpers/BinaryTree.h"
#include "ExprNode.h"

#define EXPECT_OPERATOR(node, operator) (node->getVal().getType() == TP_OPR) && \
                                        (node->getVal().getOperator().getCode() == operator)

namespace LaTEXDumper {
    static void dumpTree(FILE *output, BinaryTree<ExprNode> *head, bool paranthesis = true) {
        if (!head)
            return;
        if (head->getVal().getType() == TP_VAR) {
            fprintf(output, "%c", head->getVal().getVar());
            return;
        } else if (head->getVal().getType() == TP_CST) {
            fprintf(output, "%.20g", head->getVal().getConst());
            return;
        }
        switch (head->getVal().getOperator().getCode()) {
            case OP_SUB: {
                if (paranthesis)
                    fprintf(output, "(");
                dumpTree(output, head->getLeft(), false);
                fprintf(output, " - ");
                dumpTree(output, head->getRight(), false);
                if (paranthesis)
                    fprintf(output, ")");
                break;
            }
            case OP_ADD: {
                if (paranthesis)
                    fprintf(output, "(");
                dumpTree(output, head->getLeft(), false);
                fprintf(output, " + ");
                dumpTree(output, head->getRight(), false);
                if (paranthesis)
                    fprintf(output, ")");
                break;
            }
            case OP_DIV: {
                fprintf(output, "\\frac{");
                dumpTree(output, head->getLeft(), false);
                fprintf(output, "}{");
                dumpTree(output, head->getRight(), false);
                fprintf(output, "}");
                break;
            }
            case OP_MUL: {
                if (paranthesis)
                    fprintf(output, "(");
                if (head->getLeft()->getVal().getType() == TP_VAR) {
                    dumpTree(output, head->getRight(), !EXPECT_OPERATOR(head->getRight(), OP_MUL));
                    fprintf(output, " \\cdot ");
                    dumpTree(output, head->getLeft(), !EXPECT_OPERATOR(head->getLeft(), OP_MUL));
                } else {
                    dumpTree(output, head->getLeft(), !EXPECT_OPERATOR(head->getLeft(), OP_MUL));
                    fprintf(output, " \\cdot ");
                    dumpTree(output, head->getRight(), !EXPECT_OPERATOR(head->getRight(), OP_MUL));
                }
                if (paranthesis)
                    fprintf(output, ")");
                break;
            }
            case OP_EXP: {
                fprintf(output, "(");
                dumpTree(output, head->getLeft(), false);
                fprintf(output, ")^{");
                dumpTree(output, head->getRight(), false);
                fprintf(output, "}");
                break;
            }
            case OP_SIN: {
                fprintf(output, "\\sin{");
                dumpTree(output, head->getRight(), false);
                fprintf(output, "}");
                break;
            }
            case OP_COS: {
                fprintf(output, "\\cos{");
                dumpTree(output, head->getRight(), false);
                fprintf(output, "}");
                break;
            }
            case OP_TAN: {
                fprintf(output, "\\tan{");
                dumpTree(output, head->getRight(), false);
                fprintf(output, "}");
                break;
            }
            case OP_CTG: {
                fprintf(output, "\\cot{");
                dumpTree(output, head->getRight(), false);
                fprintf(output, "}");
                break;
            }
            case OP_LOG: {
                fprintf(output, "\\log{");
                dumpTree(output, head->getRight(), false);
                fprintf(output, "}");
                break;
            }
            case OP_ATAN: {
                fprintf(output, "\\arctan{");
                dumpTree(output, head->getRight(), false);
                fprintf(output, "}");
                break;
            }
            case OP_ACOS: {
                fprintf(output, "\\arccos{");
                dumpTree(output, head->getRight(), false);
                fprintf(output, "}");
                break;
            }
            case OP_ASIN: {
                fprintf(output, "\\arcsin{");
                dumpTree(output, head->getRight(), false);
                fprintf(output, "}");
                break;
            }
            case OP_ACTG: {
                fprintf(output, "\\arccot{");
                dumpTree(output, head->getRight(), false);
                fprintf(output, "}");
                break;
            }
        }
    }

    static void dumpDocStart(FILE *output) {
        fprintf(output, "\\documentclass[11pt, oneside]{article}\n"
                        "\\usepackage{amsmath,amsthm,amssymb}\n"
                        "\\usepackage{mathtext}\n"
                        "\\usepackage[T1,T2A]{fontenc}\n"
                        "\\usepackage[utf8]{inputenc}\n"
                        "\\usepackage[english,bulgarian,ukrainian,russian]{babel}\n"
                        "\\usepackage{algorithm2e}\n"
                        "\\usepackage{indentfirst}\n"
                        "\\setlength{\\parindent}{15pt}\n"
                        "\\begin{document}");
    }

    static void dumpDocEnd(FILE *output) {
        fprintf(output, "\\end{document}");
    }

    static void dumpFullLaTEX(FILE *output, BinaryTree<ExprNode> *head) {
        dumpDocStart(output);
        fprintf(output, "\\[");
        dumpTree(output, head, false);
        fprintf(output, "\\]");
        dumpDocEnd(output);
    }
}

#endif //DIFFERENTIATOR_LATEXDUMPER_H
