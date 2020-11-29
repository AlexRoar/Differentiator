#include <cstdio>
#include "Derivation/Differentiator.h"


int main() {
    const int maxExprLen = 2048;
    FILE *file = fopen("output/latex.tex", "w");

    printf("Enter file name:\n> [expr.txt] ");

    char *fileName = static_cast<char *>(calloc(maxExprLen, sizeof(char)));
    scanf("%s", fileName);

    if (fileName[0] == 'y' && fileName[1] == '\0'){
        strcpy(fileName, "expr.txt");
    }

    char *expression = LaTEXPhrases::readFile(fileName);
    if (expression == nullptr)
        return EXIT_FAILURE;

    free(fileName);

    Differentiator::ArticleGenerator(file, expression);

    fclose(file);
    free(expression);
    system("pdflatex -output-directory output/ -output-format=pdf output/latex.tex");
    return 0;
}
