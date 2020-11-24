#include "InfixParser.h"
#include "BinaryTree.h"
#include "ExprNode.h"

int main() {
    auto* graph = InfixParser::parseExpression("10 * (20 + sin(60 + tan(x + 5))) / 79");

    graph->dumpGraph();

    return 0;
}
