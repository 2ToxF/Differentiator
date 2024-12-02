DEF_OP(ADD,
{
    return _ADD(DiffNode(node->left), DiffNode(node->right));
},
{
    TexTree(node->left);
    PRINTF_TO_TEX_(" + ");
    TexTree(node->right);
})

DEF_OP(SUB,
{
    return _SUB(DiffNode(node->left), DiffNode(node->right));
},
{
    TexTree(node->left);
    PRINTF_TO_TEX_(" - ");
    TexTree(node->right);
})

DEF_OP(MUL,
{
    return  _ADD(
                _MUL(DiffNode(node->left), TreeCpy(node->right)),
                _MUL(TreeCpy(node->left), DiffNode(node->right))
                );
},
{
    if (node->left->type == OP && node->left->value.value_op <= SUB)
    {
        PRINTF_TO_TEX_("(");
        TexTree(node->left);
        PRINTF_TO_TEX_(")");
    }

    else
        TexTree(node->left);

    PRINTF_TO_TEX_(" \\cdot ");

    if (node->right->type == OP && node->right->value.value_op <= SUB)
    {
        PRINTF_TO_TEX_("(");
        TexTree(node->right);
        PRINTF_TO_TEX_(")");
    }

    else
        TexTree(node->right);
})

DEF_OP(DIV,
{
    return  _DIV(
                _SUB(
                    _MUL(DiffNode(node->left), TreeCpy(node->right)),
                    _MUL(TreeCpy(node->left), DiffNode(node->right))
                    ),
                _POW
                    (
                    TreeCpy(node->right),
                    _NUM(2)
                    )
                );
},
{
    PRINTF_TO_TEX_("\\frac{");
    TexTree(node->left);
    PRINTF_TO_TEX_("}{");
    TexTree(node->right);
    PRINTF_TO_TEX_("}");
})

DEF_OP(POW,
{
    Node_t* new_diff_node = DiffPow(node);
    if (new_diff_node != NULL)
        return new_diff_node;

    break;
},
{
    if (node->left->type != CONST && node->left->type != VAR)
    {
        PRINTF_TO_TEX_("(");
        TexTree(node->left);
        PRINTF_TO_TEX_(")");
    }

    else
        TexTree(node->left);

    PRINTF_TO_TEX_("^{");
    TexTree(node->right);
    PRINTF_TO_TEX_("}");
})
