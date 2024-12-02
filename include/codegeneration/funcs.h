DEF_OP_ONE_ARG(EXP,
{
    return _MUL(_EXP(TreeCpy(node->left)), DiffNode(node->left));
})

DEF_OP_ONE_ARG(LN,
{
    return _DIV(DiffNode(node->left), TreeCpy(node->left));
})

DEF_OP(LOG,
{
    Node_t* transformed_node = _DIV(_LN(node->right), _LN(node->left));
    Node_t* new_diff_node    = DiffNode(transformed_node);

    free(transformed_node); transformed_node = NULL;
    return new_diff_node;
})

/// ---------------------------------------------------------------------------------------------------------

DEF_OP_ONE_ARG(SIN,
{
    return  _MUL(
                DiffNode(node->left),
                _COS(TreeCpy(node->left))
                );
})

DEF_OP_ONE_ARG(COS,
{
    return  _MUL(
                _NUM(-1),
                _MUL(
                    DiffNode(node->left),
                    _SIN(TreeCpy(node->left))
                    )
                );
})

DEF_OP_ONE_ARG(TAN,
{
    return  _DIV(
                DiffNode(node->left),
                _POW(
                    _COS(TreeCpy(node->left)),
                    _NUM(2)
                    )
                );
})

DEF_OP_ONE_ARG(CTG,
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

DEF_OP_ONE_ARG(SH,
{
    return  _MUL(
                DiffNode(node->left),
                _CH(TreeCpy(node->left))
                );
})

DEF_OP_ONE_ARG(CH,
{
    return  _MUL(
                DiffNode(node->left),
                _SH(TreeCpy(node->left))
                );
})

DEF_OP_ONE_ARG(TH,
{
    return  _DIV(
                DiffNode(node->left),
                _POW(
                    _CH(TreeCpy(node->left)),
                    _NUM(2)
                    )
                );
})

DEF_OP_ONE_ARG(CTH,
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

DEF_OP_ONE_ARG(ARCSIN,
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

DEF_OP_ONE_ARG(ARCCOS,
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

DEF_OP_ONE_ARG(ARCTAN,
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

DEF_OP_ONE_ARG(ARCCTG,
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

DEF_OP_ONE_ARG(ARCSH,
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

DEF_OP_ONE_ARG(ARCCH,
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

DEF_OP_ONE_ARG(ARCTH,
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

DEF_OP_ONE_ARG(ARCCTH,
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
