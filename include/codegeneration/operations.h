DEF_OP(ADD,
{
    return _ADD(DiffNode(node->left, var_diff_by), DiffNode(node->right, var_diff_by));
},
{
    TexTree(tex_file_ptr, node->left);
    PRINTF_TO_TEX_(" + ");
    TexTree(tex_file_ptr, node->right);
})

DEF_OP(SUB,
{
    return _SUB(DiffNode(node->left, var_diff_by), DiffNode(node->right, var_diff_by));
},
{
    TexTree(tex_file_ptr, node->left);
    PRINTF_TO_TEX_(" - ");
    TexTree(tex_file_ptr, node->right);
})

DEF_OP(MUL,
{
    return  _ADD(
                _MUL(DiffNode(node->left, var_diff_by), TreeCpy(node->right)),
                _MUL(TreeCpy(node->left), DiffNode(node->right, var_diff_by))
                );
},
{
    if (node->left->type == OP && node->left->value.value_op <= SUB)
    {
        PRINTF_TO_TEX_("(");
        TexTree(tex_file_ptr, node->left);
        PRINTF_TO_TEX_(")");
    }

    else
        TexTree(tex_file_ptr, node->left);

    PRINTF_TO_TEX_(" \\cdot ");

    if (node->right->type == OP && node->right->value.value_op <= SUB)
    {
        PRINTF_TO_TEX_("(");
        TexTree(tex_file_ptr, node->right);
        PRINTF_TO_TEX_(")");
    }

    else
        TexTree(tex_file_ptr, node->right);
})

DEF_OP(DIV,
{
    return  _DIV(
                _SUB(
                    _MUL(DiffNode(node->left, var_diff_by), TreeCpy(node->right)),
                    _MUL(TreeCpy(node->left), DiffNode(node->right, var_diff_by))
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
    TexTree(tex_file_ptr, node->left);
    PRINTF_TO_TEX_("}{");
    TexTree(tex_file_ptr, node->right);
    PRINTF_TO_TEX_("}");
})

DEF_OP(POW,
{
    Node_t* new_diff_node = DiffPow(node, var_diff_by);
    if (new_diff_node != NULL)
        return new_diff_node;

    break;
},
{
    if (node->right->type == CONST && IsEqual(node->right->value.value_const, 0.5))
    {
        PRINTF_TO_TEX_("\\sqrt{");
        TexTree(tex_file_ptr, node->left);
        PRINTF_TO_TEX_("}");
    }

    else
    {
        if (node->left->type == OP)
        {
            PRINTF_TO_TEX_("(");
            TexTree(tex_file_ptr, node->left);
            PRINTF_TO_TEX_(")");
        }

        else
            TexTree(tex_file_ptr, node->left);

        PRINTF_TO_TEX_("^{");
        TexTree(tex_file_ptr, node->right);
        PRINTF_TO_TEX_("}");
    }
})
