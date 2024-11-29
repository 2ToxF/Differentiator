DEF_OP_ONE_ARG(EXP, 1, FUNCTION,
{
    return _MUL(_EXP(TreeCpy(node->left)), DiffNode(node->left));
})

DEF_OP_ONE_ARG(LN, 1, FUNCTION,
{
    return _DIV(DiffNode(node->left), TreeCpy(node->left));
})

DEF_OP(POW, 2, FUNCTION,
{
    Node_t* new_diff_node = DiffPow(node);
    if (new_diff_node != NULL)
        return new_diff_node;

    break;
})

DEF_OP(LOG, 2, FUNCTION,
{
    Node_t* transformed_node = _DIV(_LN(node->right), _LN(node->left));
    Node_t* new_diff_node    = DiffNode(transformed_node);

    free(transformed_node); transformed_node = NULL;
    return new_diff_node;
})

/// ---------------------------------------------------------------------------------------------------------

DEF_OP_ONE_ARG(SIN, 1, FUNCTION,
{
    return  _MUL(
                DiffNode(node->left),
                _COS(TreeCpy(node->left))
                );
})

DEF_OP_ONE_ARG(COS, 1, FUNCTION,
{
    return  _MUL(
                _NUM(-1),
                _MUL(
                    DiffNode(node->left),
                    _SIN(TreeCpy(node->left))
                    )
                );
})

DEF_OP_ONE_ARG(TAN, 1, FUNCTION,
{
    return  _DIV(
                DiffNode(node->left),
                _POW(
                    _COS(TreeCpy(node->left)),
                    _NUM(2)
                    )
                );
})

DEF_OP_ONE_ARG(CTG, 1, FUNCTION,
{
    return _DIV(
                _MUL(
                    _NUM(-1),
                    DiffNode(node->left)
                    ),
                _POW(
                    _SIN(TreeCpy(node->left)),
                    _NUM(2)
                    )
                );
})

/// ---------------------------------------------------------------------------------------------------------

DEF_OP_ONE_ARG(SH, 1, FUNCTION,
{
    return  _MUL(
                DiffNode(node->left),
                _CH(TreeCpy(node->left))
                );
})

DEF_OP_ONE_ARG(CH, 1, FUNCTION,
{
    return  _MUL(
                DiffNode(node->left),
                _SH(TreeCpy(node->left))
                );
})

DEF_OP_ONE_ARG(TH, 1, FUNCTION,
{
    return  _DIV(
                DiffNode(node->left),
                _POW(
                    _CH(TreeCpy(node->left)),
                    _NUM(2)
                    )
                );
})

DEF_OP_ONE_ARG(CTH, 1, FUNCTION,
{
    return  _DIV(
                _MUL(
                    _NUM(-1),
                    DiffNode(node->left)
                    ),
                _POW(
                    _SH(TreeCpy(node->left)),
                    _NUM(2)
                    )
                );
})

/// ---------------------------------------------------------------------------------------------------------

DEF_OP_ONE_ARG(ARCSIN, 1, FUNCTION,
{
    return  _DIV(
                DiffNode(node->left),
                _POW(
                    _SUB(
                        _NUM(1),
                        _POW(
                            TreeCpy(node->left),
                            _NUM(2)
                            )
                        ),
                    _DIV(_NUM(1), _NUM(2))
                    )
                );
})

DEF_OP_ONE_ARG(ARCCOS, 1, FUNCTION,
{
    return  _DIV(
                _MUL(
                    _NUM(-1),
                    DiffNode(node->left)
                    ),
                _POW(
                    _SUB(
                        _NUM(1),
                        _POW(
                            TreeCpy(node->left),
                            _NUM(2)
                            )
                        ),
                    _DIV(_NUM(1), _NUM(2))
                    )
                );
})

DEF_OP_ONE_ARG(ARCTAN, 1, FUNCTION,
{
    return  _DIV(
                DiffNode(node->left),
                _ADD(
                    _NUM(1),
                    _POW(
                        TreeCpy(node->left),
                        _NUM(2)
                        )
                    )
                );
})

DEF_OP_ONE_ARG(ARCCTG, 1, FUNCTION,
{
    return  _DIV(
                _MUL(
                    _NUM(-1),
                    DiffNode(node->left)
                    ),
                _ADD(
                    _NUM(1),
                    _POW(
                        TreeCpy(node->left),
                        _NUM(2)
                        )
                    )
                );
})

/// ---------------------------------------------------------------------------------------------------------

DEF_OP_ONE_ARG(ARCSH, 1, FUNCTION,
{
    return  _DIV(
                DiffNode(node->left),
                _POW(
                    _ADD(
                        _POW(
                            TreeCpy(node->left),
                            _NUM(2)
                            ),
                        _NUM(1)
                        ),
                    _DIV(_NUM(1), _NUM(2))
                    )
                );
})

DEF_OP_ONE_ARG(ARCCH, 1, FUNCTION,
{
    return  _DIV(
                DiffNode(node->left),
                _POW(
                    _SUB(
                        _POW(
                            TreeCpy(node->left),
                            _NUM(2)
                            ),
                        _NUM(1)
                        ),
                    _DIV(_NUM(1), _NUM(2))
                    )
                );
})

DEF_OP_ONE_ARG(ARCTH, 1, FUNCTION,
{
    return  _DIV(
                DiffNode(node->left),
                _SUB(
                    _NUM(1),
                    _POW(
                        TreeCpy(node->left),
                        _NUM(2)
                        )
                    )
                );
})

DEF_OP_ONE_ARG(ARCCTH, 1, FUNCTION,
{
    return  _DIV(
                DiffNode(node->left),
                _SUB(
                    _NUM(1),
                    _POW(
                        TreeCpy(node->left),
                        _NUM(2)
                        )
                    )
                );
})
