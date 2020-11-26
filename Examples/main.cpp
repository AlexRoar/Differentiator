#include <cstdio>
#include "Derivation/Differentiator.h"

// TODO: compare trees
int main() {
    FILE *file = fopen("latex.tex", "w");

    Differentiator::ArticleGenerator(file, "(x^2 + 17 * x + x^x + arctan(x^x) ^ x + cos(89 * x - 3))^(sin(x) * x ^ 3)");

    fclose(file);
    system("pdflatex -output-format=pdf latex.tex");
    return 0;
}
