//
// Created by Александр Дремов on 06.12.2020.
//

#define EVAL_L Evaluator::eval(head->getLeft(), evalMath)
#define EVAL_R Evaluator::eval(head->getRight(), evalMath)

#define EXPECT_EVAL_R_EQ(val) (evalR.status == EV_OK && evalR.res == val)
#define EXPECT_EVAL_L_EQ(val) (evalL.status == EV_OK && evalL.res == val)

#define LA_DUMPED(code) { \
                            unsigned before = changed; \
                            BinaryTree<ExprNode>* copy = nullptr;\
                            if (laFile)\
                                 copy = head->deepCopy();      \
                            {code}                    \
                            if (before != changed && laFile){    \
                                LaTEXDumper::rawWrite(laFile, LaTEXPhrases::primarySimplifyStartCasesRandom()); \
                                LaTEXDumper::dumpTreeBlock(laFile, copy);       \
                                LaTEXDumper::rawWrite(laFile, LaTEXPhrases::primarySimplifyEndCasesRandom()); \
                                LaTEXDumper::dumpTreeBlock(laFile, head);       \
                            }\
                            BinaryTree<ExprNode>::Delete(copy);\
                            break;  \
                        }
