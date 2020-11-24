#include "InfixParser.h"
#include "BinaryTree.h"
#include "ExprNode.h"

int main() {
    InfixParser parser{};
    parser.cTor();

   ClassicStack<ExprNode> tokens = parser.tokenize("1 + 2 * sin(3 + 6)");

    return 0;
}
