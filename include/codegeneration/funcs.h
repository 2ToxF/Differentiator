DEF_OP_ONE_ARG(EXP,
{
    return _MUL(_EXP(TreeCpy(node->left)), DiffNode(node->left));
},
{
    PRINTF_TO_TEX_("e^{");
    TexTree(tex_file_ptr, node->left);
    PRINTF_TO_TEX_("}");
})

DEF_OP_ONE_ARG(LN,
{
    return _DIV(DiffNode(node->left), TreeCpy(node->left));
},
{
    PRINTF_TO_TEX_("ln(");
    TexTree(tex_file_ptr, node->left);
    PRINTF_TO_TEX_(")");
})

DEF_OP(LOG,
{
    if (node->left->type == CONST)
    {
        Node_t* node_to_diff  = _LN(node->right);
        Node_t* new_diff_node = DiffNode(node_to_diff);

        free(node_to_diff); node_to_diff = NULL;
        return  _MUL(
                    _DIV(
                        _NUM(1),
                        _LN(node->left)
                        ),
                    new_diff_node
                    );
    }

    else
    {
        Node_t* transformed_node = _DIV(_LN(node->right), _LN(node->left));
        Node_t* new_diff_node    = DiffNode(transformed_node);

        free(transformed_node); transformed_node = NULL;
        return new_diff_node;
    }
},
{
    PRINTF_TO_TEX_("log_{");
    TexTree(tex_file_ptr, node->left);
    PRINTF_TO_TEX_("}");

    PRINTF_TO_TEX_("(");
    TexTree(tex_file_ptr, node->right);
    PRINTF_TO_TEX_(")");
})

/// ---------------------------------------------------------------------------------------------------------

DEF_OP_ONE_ARG(SIN,
{
    return  _MUL(
                DiffNode(node->left),
                _COS(TreeCpy(node->left))
                );
},
{
    PRINTF_TO_TEX_("sin(");
    TexTree(tex_file_ptr, node->left);
    PRINTF_TO_TEX_(")");
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
},
{
    PRINTF_TO_TEX_("cos(");
    TexTree(tex_file_ptr, node->left);
    PRINTF_TO_TEX_(")");
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
},
{
    PRINTF_TO_TEX_("tg(");
    TexTree(tex_file_ptr, node->left);
    PRINTF_TO_TEX_(")");
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
},
{
    PRINTF_TO_TEX_("ctg(");
    TexTree(tex_file_ptr, node->left);
    PRINTF_TO_TEX_(")");
})

/// ---------------------------------------------------------------------------------------------------------

DEF_OP_ONE_ARG(SH,
{
    return  _MUL(
                DiffNode(node->left),
                _CH(TreeCpy(node->left))
                );
},
{
    PRINTF_TO_TEX_("sh(");
    TexTree(tex_file_ptr, node->left);
    PRINTF_TO_TEX_(")");
})

DEF_OP_ONE_ARG(CH,
{
    return  _MUL(
                DiffNode(node->left),
                _SH(TreeCpy(node->left))
                );
},
{
    PRINTF_TO_TEX_("ch(");
    TexTree(tex_file_ptr, node->left);
    PRINTF_TO_TEX_(")");
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
},
{
    PRINTF_TO_TEX_("th(");
    TexTree(tex_file_ptr, node->left);
    PRINTF_TO_TEX_(")");
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
},
{
    PRINTF_TO_TEX_("cth(");
    TexTree(tex_file_ptr, node->left);
    PRINTF_TO_TEX_(")");
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
},
{
    PRINTF_TO_TEX_("arcsin(");
    TexTree(tex_file_ptr, node->left);
    PRINTF_TO_TEX_(")");
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
},
{
    PRINTF_TO_TEX_("arccos(");
    TexTree(tex_file_ptr, node->left);
    PRINTF_TO_TEX_(")");
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
},
{
    PRINTF_TO_TEX_("arctan(");
    TexTree(tex_file_ptr, node->left);
    PRINTF_TO_TEX_(")");
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
},
{
    PRINTF_TO_TEX_("arcctg(");
    TexTree(tex_file_ptr, node->left);
    PRINTF_TO_TEX_(")");
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
},
{
    PRINTF_TO_TEX_("arcsh(");
    TexTree(tex_file_ptr, node->left);
    PRINTF_TO_TEX_(")");
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
},
{
    PRINTF_TO_TEX_("arcch(");
    TexTree(tex_file_ptr, node->left);
    PRINTF_TO_TEX_(")");
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
},
{
    PRINTF_TO_TEX_("arcth(");
    TexTree(tex_file_ptr, node->left);
    PRINTF_TO_TEX_(")");
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
},
{
    PRINTF_TO_TEX_("arccth(");
    TexTree(tex_file_ptr, node->left);
    PRINTF_TO_TEX_(")");
})
