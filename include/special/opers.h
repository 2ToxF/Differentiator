DEF_OP(ADD, 2, OPERATION,
{
    return _ADD(DiffNode(node->left), DiffNode(node->right));
})

DEF_OP(SUB, 2, OPERATION,
{
    return _SUB(DiffNode(node->left), DiffNode(node->right));
})

DEF_OP(MUL, 2, OPERATION,
{
    return  _ADD(
                _MUL(DiffNode(node->left), TreeCpy(node->right)),
                _MUL(TreeCpy(node->left), DiffNode(node->right))
                );
})

DEF_OP(DIV, 2, OPERATION,
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
