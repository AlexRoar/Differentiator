//
// Created by Александр Дремов on 26.11.2020.
//

#ifndef DIFFERENTIATOR_LATEXPHRASES_H
#define DIFFERENTIATOR_LATEXPHRASES_H

struct LaTEXPhrases {
public:
    static const char* getDocStart() {
        if (docStart == nullptr)
            docStart = readFile("files/docStart.txt");
        return docStart;
    }

    static const char* getIntro() {
        if (intro == nullptr)
            intro = readFile("files/intro.txt");
        return intro;
    }

    static const char* getConclusion() {
        if (conclusion == nullptr)
            conclusion = readFile("files/conclusion.txt");
        return conclusion;
    }

    static const char* getPrimarySimplify() {
        if (primarySimplify == nullptr)
            primarySimplify = readFile("files/primarySimplify.txt");
        return primarySimplify;
    }

    static const char* getPrimarySimplifyEnd() {
        if (primarySimplifyEnd == nullptr)
            primarySimplifyEnd = readFile("files/primarySimplifyEnd.txt");
        return primarySimplifyEnd;
    }

    static const char* getPrimaryDiff() {
        if (primaryDiff == nullptr)
            primaryDiff = readFile("files/primaryDiff.txt");
        return primaryDiff;
    }

    static const char* getPrimaryDiffEnd() {
        if (primaryDiffEnd == nullptr)
            primaryDiffEnd = readFile("files/primaryDiffEnd.txt");
        return primaryDiffEnd;
    }

    static const char* getSecondSimplify() {
        if (secondSimplify == nullptr)
            secondSimplify = readFile("files/secondSimplify.txt");
        return secondSimplify;
    }

    static const char* getSecondSimplifyEnd() {
        if (secondSimplifyEnd == nullptr)
            secondSimplifyEnd = readFile("files/secondSimplifyEnd.txt");
        return secondSimplifyEnd;
    }

    static const char* getFullDerivative() {
        if (fullDerivative == nullptr)
            fullDerivative = readFile("files/fullDerivative.txt");
        return fullDerivative;
    }

    static const char* getFullDerivativeSimplify() {
        if (fullDerivativeSimplify == nullptr)
            fullDerivativeSimplify = readFile("files/fullDerivativeSimplify.txt");
        return fullDerivativeSimplify;
    }

    static const char* getFullDerivativeResult() {
        if (fullDerivativeResult == nullptr)
            fullDerivativeResult = readFile("files/fullDerivativeResult.txt");
        return fullDerivativeResult;
    }

    static const char *primarySimplifyStartCasesRandom() {
        return primarySimplifyStartCases[rand() % sizeof((primarySimplifyStartCases)) /
                                         sizeof((primarySimplifyStartCases[0]))];
    }

    static const char *primarySimplifyEndCasesRandom() {
        return primarySimplifyEndCases[rand() % sizeof((primarySimplifyEndCases)) /
                                       sizeof((primarySimplifyEndCases[0]))];
    }

    static const char *primaryDiffStartCasesRandom() {
        return primaryDiffStartCases[rand() % sizeof((primaryDiffStartCases)) /
                                     sizeof((primaryDiffStartCases[0]))];
    }

    static const char *primaryDiffEndCasesRandom() {
        return primaryDiffEndCases[rand() % sizeof((primaryDiffEndCases)) /
                                   sizeof((primaryDiffEndCases[0]))];
    }

    constexpr static const char *primarySimplifyStartCases[] = {
            "Проведя простейший анализ элементарного состояния вещества",
            "Заметим, что в данном случае тривиальным является",
            "Данный переход оставляется читателю на самостоятельное изучение",
            "Применим теорему о булевых пифагоровых тройках",
            "Рассмотрев выражение с позиции n-мерных кубов"
    };


    constexpr static const char *primarySimplifyEndCases[] = {
            "приходим к эквивалентному выражению",
            "замечаем что выражение эквивалентно",
            "преобразуем в равное исходному",
            "применяя метод логарифмирования и возведения e в степень выражения приходим к",
            "выполнив простую подстановку получим выражение"
    };

    constexpr static const char *primaryDiffStartCases[] = {
            "Необходимо вычислить производную",
            "Для этого вычислим производную",
            "Очевидно, что необходимо вычислить производную",
            "Необходима производная следующего выражения",
    };

    constexpr static const char *primaryDiffEndCases[] = {
            "которое простейшей операцией преобразуется в",
            "легко превращается в",
            "применив стандартные методы дифференцирования получим",
            "легко преобразовывается в",
    };

    static void freeSpace() {
        freeIfAlocated(docStart);
        freeIfAlocated(intro);
        freeIfAlocated(primarySimplify);
        freeIfAlocated(primarySimplifyEnd);
        freeIfAlocated(primaryDiff);
        freeIfAlocated(primaryDiffEnd);
        freeIfAlocated(secondSimplify);
        freeIfAlocated(secondSimplifyEnd);
        freeIfAlocated(fullDerivative);
        freeIfAlocated(fullDerivativeResult);
        freeIfAlocated(fullDerivativeSimplify);
        freeIfAlocated(conclusion);
    }

    static char *readFile(const char* name) {
        FILE* contentFile = fopen(name, "r");

        if (!contentFile) {
            printf("Can't open file \"%s\"\n", name);
            return nullptr;
        }

        fseek(contentFile, 0, SEEK_END);
        size_t fsize = ftell(contentFile);
        fseek(contentFile, 0, SEEK_SET);

        char* content = static_cast<char*>(calloc(fsize + 1, sizeof(char)));
        fread(content, 1, fsize, contentFile);
        fclose(contentFile);

        return content;
    }
private:
    static void freeIfAlocated(void* ptr){
        if (!ptr) return;
        free(ptr);
    }
    static char* docStart;
    static char* intro;
    static char* primarySimplify;
    static char* primarySimplifyEnd;
    static char* primaryDiff;
    static char* primaryDiffEnd;
    static char* secondSimplify;
    static char* secondSimplifyEnd;
    static char* fullDerivative;
    static char* fullDerivativeResult;
    static char* fullDerivativeSimplify;
    static char* conclusion;
};

char* LaTEXPhrases::docStart              = nullptr;
char* LaTEXPhrases::intro                 = nullptr;
char* LaTEXPhrases::primarySimplify       = nullptr;
char* LaTEXPhrases::primarySimplifyEnd    = nullptr;
char* LaTEXPhrases::primaryDiff           = nullptr;
char* LaTEXPhrases::primaryDiffEnd        = nullptr;
char* LaTEXPhrases::secondSimplify        = nullptr;
char* LaTEXPhrases::secondSimplifyEnd     = nullptr;
char* LaTEXPhrases::fullDerivative        = nullptr;
char* LaTEXPhrases::fullDerivativeResult  = nullptr;
char* LaTEXPhrases::conclusion            = nullptr;
char* LaTEXPhrases::fullDerivativeSimplify  = nullptr;

#endif //DIFFERENTIATOR_LATEXPHRASES_H
