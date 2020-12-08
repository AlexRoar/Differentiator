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
                                        (node->getVal().getOperator() < op)

#define EXPECT_OPERATOR(node) (node->getVal().getType() == TP_OPR)

#define EXPECT_OPERATOR_LOWER_OR_EQ(node, op) (node->getVal().getType() == TP_OPR) && \
                                              (node->getVal().getOperator() <= op)
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
                dumpTree(output, head->getLeft(), false);
                fprintf(output, "}{");
                dumpTree(output, head->getRight(), false);
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
                    dumpTree(output, head->getLeft(), EXPECT_OPERATOR(head->getLeft()));
                    fprintf(output, "}^{");
                    dumpTree(output, head->getRight(), EXPECT_OPERATOR(head->getRight()));
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
#include "Syntax/Syntax.h"
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

    void dumpGraph(FILE *output, BinaryTree<ExprNode> *head, char varName='x') {
        const double step = 0.01;
        const double minX = -10;
        const double maxX = 10;
        const double tick = 1;

        fprintf(output, "\\begin{center}"
                        "\\begin{scaletikzpicturetowidth}{\\textwidth}"
                        "\\begin{tikzpicture}[scale=\\tikzscale]\n"
                        "\t\\begin{axis}[\n"
                        "\t\txlabel = {$%c$},\n"
                        "\t\tminor tick num = 5\n"
                        "\t\t]"
                        "xtick={\n", varName);
        for (int i = minX; i <= maxX; i+=tick) {
            fprintf(output, "%d", i);
            if (i != maxX)
                fprintf(output, ", ");
        }
        fprintf(output, "},\n"
                        "    ytick={1},\n"
                        "    legend pos=north west,\n"
                        "    ymajorgrids=true,\n"
                        "    grid style=dashed,\n"
                        "]\n"
                        "\n"
                        "\\addplot[\n"
                        "    color=red,\n"
                        "    mark=none,\n"
                        "    ]\n"
                        "    coordinates {\n");

        double vars[26 * 2] = {};
        for (double i = minX; i <= maxX; i+=step) {
            fprintf(output, "(%lf, ", i);

            vars[varName - 'a'] = i;
            EvaluatorRes res = Evaluator::eval(head, true, true, vars);
            if (res.status != EV_OK){
                printf("Error evaluating expression: %d\n", res.status);
            }

            fprintf(output, "%lf", res.res);
            fprintf(output, ")\n");
        }
        fprintf(output, "\n");

        fprintf(output,
                        "    };\n"
                        "    \\legend{}\n"
                        "    \n"
                        "\\end{axis}\n"
                        "\\end{tikzpicture}\n"
                        "\\end{scaletikzpicturetowidth}"
                        "\\end{center}\n\n");
    }

    void rawWrite(FILE *output, const char *content) {
        if (output)
            fprintf(output, "%s\n", content);
    }
}

#endif //DIFFERENTIATOR_LATEXDUMPER_H
