#include "Helpers/InfixParser.h"
#include "Derivation/Differentiator.h"
#include "LaTEX/LaTEXDumper.h"

// TODO: compare trees
int main() {
    ExprOptimizer optimizer{};
    auto* graph = InfixParser::parseExpression("x + 9 / 6 + 5 / (2 * x )");
    graph->dumpGraph();
    optimizer.cTor(graph);

    optimizer.simplify(false);
    graph = optimizer.getStructure();
    graph->dumpGraph();

    FILE* file = fopen("latex.tex", "w");
    LaTEXDumper::dumpFullLaTEX(file, graph);
    fclose(file);
    system("pdflatex -output-format=pdf latex.tex");

    auto* derived = Differentiator::derivative(graph, 'x');
    derived->dumpGraph();
    optimizer.cTor(derived);

    optimizer.simplify(false);
    optimizer.getStructure()->dumpGraph();
    return 0;
}
