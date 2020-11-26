//
// Created by Александр Дремов on 26.11.2020.
//

#ifndef DIFFERENTIATOR_LATEXPHRASES_H
#define DIFFERENTIATOR_LATEXPHRASES_H

struct LaTEXPhrases {
    constexpr static const char *docStart = "\t\\begin{titlepage}                                                         \n"
                                            "\t\t\\newpage                                                                        \n"
                                            "\t\t\\begin{center}                                                        \n"
                                            "\t\t\t{\\bfseries Министерство Образования и Науки Российской Федерации \\\\\n"
                                            "\t\t\t\tМосковский Физико Технический Институт}                               \n"
                                            "\t\t\t\\vspace{1cm}                                                          \n"
                                            "                                       \n"
                                            "\t\t\tКафедра численных методов                                                         \n"
                                            "\t\t\t\\vspace{6em}                                                          \n"
                                            "\t\t\t                                             \n"
                                            "\t\t\tДремов Александр Олегович\\\\                                                      \n"
                                            "\t\t\\end{center}                                                          \n"
                                            "\t\t\n"
                                            "\t\t\\vspace{1.2em}                                                        \n"
                                            "\t\t\n"
                                            "\t\t\\begin{center}                                                        \n"
                                            "\t\t\t\\Large{\\textbf{Расчет производной сложной функции с применением методов квантовой запутанности}}                                                                                                \n"
                                            "\t\t\\end{center}                                                          \n"
                                            "\t\t\n"
                                            "\t\t\\vspace{5em}                                                          \n"
                                            "\t\t\n"
                                            "\t\t\\begin{center}                                                                                                                          \n"
                                            "\t\t\tкандидатская диссертация                                                           \n"
                                            "\t\t\\end{center}                                                         \n"
                                            "\t\t\\vspace{6em}                                                          \n"
                                            "\t\t\n"
                                            "\t\t\\vspace{\\fill}                                                    \n"
                                            "\t\t\n"
                                            "\t\t\\begin{center}                                                        \n"
                                            "\t\t\tМосква 2020                                                                \n"
                                            "\t\t\\end{center}                                                          \n"
                                            "\t\t\n"
                                            "\t\\end{titlepage}\n\n";
    constexpr static const char *intro = "\\section{\\Large{Введение}}\n"
                                         "\n"
                                         "В современном мире с развитием технологий подгона и вычислений в "
                                         "системах быстрых отображений, появилось необходимость развития "
                                         "сферы действия запутанных и квази-запутанных алгоритмов (от англ. lorem ipsum). "
                                         "Так, при рассмотрении предложенной темы были найдены редкие примеры отличные от "
                                         "всех остальных по максимально допустимой энтропии.\n"
                                         "\n"
                                         "Так как данная тема является основной при изучении запутанности материи построенной "
                                         "на опорных векторах элементарных систем, было принято решеие досканально изучить выбивающиеся из общего представления выражения.\n"
                                         "\n"
                                         "В данной работе предложен метод преобразования сложной функции для получения необходимого результата."
                                         " Более того, предложенный метод отличается на 10 процентов эффективностью от своих аналогов, которых найти не удалось.\n"
                                         "\n"
                                         "\\section{\\Large{Открытие}}\n"
                                         "Уникальным в своем роде является следующее выражение. Далее, будем рассматривать именно его.\n\n";


    constexpr static const char *primarySimplify = "\\section{\\Large{Упрощение исходного}}\n"
                                                   "Для выполнения квази-запутанных операций менее запутанно, упростим топологическую основу данного выражения.\n\n";

    constexpr static const char *primarySimplifyEnd = "Итого, получено выражение в простате которого сомневаться не приходится.\n\n";

    constexpr static const char *primaryDiff = "\\section{\\Large{Дифференцирование}}\n"
                                               "Перейдём же к основной теме данной статьи."
                                               "В последующих строчках будут произвелены преобразования которые одним своим существованием "
                                               "повышают энтропию сильнее хаоса нашей солнечной системы. \n\n";

    constexpr static const char *primaryDiffEnd = "Итого пришли к следующему выражению. "
                                                  "К этому моменту читатель уже должен был догадаться почему именно это "
                                                  "выражение несет в себе такую большую смысловую нагрузку. "
                                                  "Если догадаться не удалось, перечитайте все с начала.";

    constexpr static const char *secondSimplify = "\\section{\\Large{Упрощение полученного}}\n"
                                                  "Для получения истинной сути выражения, необходимо провести упрощение "
                                                  "которое расскроет весь потенциал рассматриваемой зависимости.\n\n";

    constexpr static const char *secondSimplifyEnd = "Полученное выражение несет в себе огромный смысл для численных "
                                                     "методов изучения топологии пространств.";

    constexpr static const char *primarySimplifyStartCases[] = {
            "Проведя простейший анализ элементарного состояния вещества",
            "Заметим, что в данном случае тривиальным является",
            "Данный переход оставляется читателю на самостоятельное изучение",
            "Применим теорему о булевых пифагоровых тройках",
            "Рассмотрев выражение с позиции n-мерных кубов"
    };

    static const char *primarySimplifyStartCasesRandom() {
        return primarySimplifyStartCases[rand() % sizeof((primarySimplifyStartCases)) /
                                         sizeof((primarySimplifyStartCases[0]))];
    }

    constexpr static const char *primarySimplifyEndCases[] = {
            "приходим к эквивалентному выражению",
            "замечаем что выражение эквивалентно",
            "преобразуем в равное исходному",
            "применяя метод логарифмирования и возведения e в степень выражения приходим к",
            "выполнив простую подстановку получим выражение"
    };

    static const char *primarySimplifyEndCasesRandom() {
        return primarySimplifyEndCases[rand() % sizeof((primarySimplifyEndCases)) /
                                       sizeof((primarySimplifyEndCases[0]))];
    }

    constexpr static const char *primaryDiffStartCases[] = {
            "Необходимо вычислить производную",
            "Для этого вычислим производную",
            "Очевидно, что необходимо вычислить производную",
            "Необходима производная следующего выражения",
    };

    static const char *primaryDiffStartCasesRandom() {
        return primaryDiffStartCases[rand() % sizeof((primaryDiffStartCases)) / sizeof((primaryDiffStartCases[0]))];
    }

    constexpr static const char *primaryDiffEndCases[] = {
            "которое простейшей операцией преобразуется в",
            "легко превращается в",
            "применив стандартные методы дифференцирования получим",
            "легко преобразовывается в",
    };

    static const char *primaryDiffEndCasesRandom() {
        return primaryDiffEndCases[rand() % sizeof((primaryDiffEndCases)) / sizeof((primaryDiffEndCases[0]))];
    }

};

#endif //DIFFERENTIATOR_LATEXPHRASES_H
