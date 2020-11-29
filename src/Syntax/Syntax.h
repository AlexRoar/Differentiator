#if 0
// DEF_FUNC(OP_CODE, string, latex, eval, derivative)
#endif

DEF_FUNC(
        OP_SIN,
        sin,
        sin,
        sin(evalR.res),
        MUL(COS(cR), dR)
        )

DEF_FUNC(
        OP_COS,
        cos,
        cos,
        cos(evalR.res),
        MUL(CONST(-1), MUL(SIN(cR), dR))
)

DEF_FUNC(
        OP_TAN,
        tan,
        tan,
        tan(evalR.res),
        MUL(DIV(CONST(1),POW(COS(cR),CONST(2))), dR)
)

DEF_FUNC(
        OP_CTG,
        ctg,
        cot,
        1 / tan(evalR.res),
        MUL(DIV( CONST(-1), POW(COS(cR), CONST(2)) ), dR)
)

DEF_FUNC(
        OP_ACOS,
        arccos,
        arccos,
        acos(evalR.res),
        MUL(DIV(CONST(-1), POW(SUB(CONST(1), POW(cR, CONST(2))), CONST(0.5))), dR)
)

DEF_FUNC(
        OP_ASIN,
        arcsin,
        arcsin,
        asin(evalR.res),
        MUL(DIV(CONST(1), POW(SUB(CONST(1), POW(cR, CONST(2))), CONST(0.5))), dR)
)

DEF_FUNC(
        OP_ATAN,
        arctan,
        arctan,
        atan(evalR.res),
        MUL(DIV(CONST(1),ADD(CONST(1), POW(cR,CONST(2)))), dR)
)


DEF_FUNC(
        OP_ACTG,
        arccot,
        arccot,
        atan(1 / evalR.res),
        MUL(DIV(CONST(-1),ADD(CONST(1), POW(cR,CONST(2)))), dR)
)

DEF_FUNC(
        OP_LOG,
        log,
        log,
        log(evalR.res),
        MUL(DIV(CONST(1), cR), dR)
)

DEF_FUNC(
        OP_LG,
        lg,
        lg,
        log10(evalR.res),
        MUL(MUL(DIV(CONST(1), cR), DIV(CONST(1), LOG(CONST(10)))), dR)
)

DEF_FUNC(
        OP_EXP,
        exp,
        exp,
        exp(evalR.res),
        MUL(EXP(cR), dR)
)

DEF_FUNC(
        OP_COSH,
        ch,
        cosh,
        cosh(evalR.res),
        MUL(SINH(cR), dR)
)

DEF_FUNC(
        OP_SINH,
        sh,
        sinh,
        sinh(evalR.res),
        MUL(COSH(cR), dR)
)

DEF_FUNC(
        OP_TANH,
        th,
        tanh,
        tanh(evalR.res),
        MUL(DIV(CONST(1),POW(COSH(cR),CONST(2))), dR)
)

DEF_FUNC(
        OP_CTGH,
        cth,
        coth,
        tanh(1 / evalR.res),
        MUL(DIV(CONST(-1),POW(SINH(cR),CONST(2))), dR)
)
