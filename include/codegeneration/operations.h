DEF_OP(ADD,
{
    return _ADD(DiffNode(node->left), DiffNode(node->right));
})

DEF_OP(SUB,
{
    return _SUB(DiffNode(node->left), DiffNode(node->right));
})

DEF_OP(MUL,
{
    return  _ADD(
                _MUL(DiffNode(node->left), TreeCpy(node->right)),
                _MUL(TreeCpy(node->left), DiffNode(node->right))
                );
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
})

DEF_OP(POW,
{
    Node_t* new_diff_node = DiffPow(node);
    if (new_diff_node != NULL)
        return new_diff_node;

    break;
})
