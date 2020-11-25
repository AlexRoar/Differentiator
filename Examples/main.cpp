#include "Helpers/InfixParser.h"
#include "Derivation/Differentiator.h"

int main() {
    auto* graph = InfixParser::parseExpression("1 + x * 6 + 10 * x");

    Differentiator differentiator{};
    differentiator.cTor(graph);

    auto* derived = differentiator.derivative('x');
    derived->dumpGraph();

    ExprOptimizer optimizer{};
    optimizer.cTor(derived);

    optimizer.simplify(false);
    optimizer.getStructure()->dumpGraph();

    return 0;
}
