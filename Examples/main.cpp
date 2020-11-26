#include <cstdio>
#include "Derivation/Differentiator.h"

// TODO: compare trees
int main() {
    FILE* file = fopen("latex.tex", "w");

    Differentiator::ArticleGenerator(file, "x * (x ^ 2)");

    fclose(file);
    system("pdflatex -output-format=pdf latex.tex");
    return 0;
}
