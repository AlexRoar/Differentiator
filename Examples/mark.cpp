#include <cstdio>
#include <ctime>
#include "Derivation/Differentiator.h"

#define TIME_MEASURED(code){ \
    clock_t begin = clock();     \
    code;clock_t end = clock();  \
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC; \
    printf("ELAPSED: %lf sec\n", elapsed_secs);                 \
}
#define TIME_LIMIT(sec) if (double(clock() - begin) / CLOCKS_PER_SEC > sec) { \
    printf("\tReached time limit! "); break;                                      \
}

int main() {
    FILE *file = fopen("output/latex.tex", "w");
    char *expression = LaTEXPhrases::readFile("expr.txt");
    if (expression == nullptr)
        return EXIT_FAILURE;

    printf("Single node size: %zu\n", sizeof(BinaryTree<ExprNode>));

    ClassicStack<ExprNode>* tokens = nullptr;
    auto *graph = InfixParser::parseExpression(expression, &tokens);
    if (graph == nullptr) {
        printf("Invalid expression!\n");
        return EXIT_FAILURE;
    }

    graph->dumpGraph("parsed.svg");

    ExprOptimizer optimizer {};
    optimizer.cTor(graph);
    optimizer.simplify(false);

    char* fName = static_cast<char*>(calloc(100, sizeof (char)));

    graph = optimizer.getStructure();
    graph->dumpGraph("parsedOpt.svg");
    for (int i = 1; i <= 10; i++){

        printf("%d derivative...\n", i);
        printf("Active nodes: %zu\n", activeNodes);
        BinaryTree<ExprNode> *derived = nullptr;
        TIME_MEASURED({
            derived = Differentiator::derivative(graph, 'x');
        });
        BinaryTree<ExprNode>::Delete(graph);

        optimizer.cTor(derived);
        printf("\t%d derivative simplify...\n", i);
        TIME_MEASURED({
                          optimizer.simplify(false, nullptr, 10);
        });
        derived = optimizer.getStructure();
        printf("\tdone\n");

        LaTEXDumper::dumpTreeBlock(file, derived);

        sprintf(fName, "derivative%d.svg", i);
        printf("done\n");
        graph = derived;

    }

    BinaryTree<ExprNode>::Delete(graph);
    printf("Active nodes: %zu\n", activeNodes);

    free(fName);
    fclose(file);
    free(expression);
    return 0;
}
