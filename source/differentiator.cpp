#include "settings.h" // !!!

#include "differentiator.h"
#include "tree.h"

static Node_t* DiffOp(Node_t* node);


Node_t* DiffNode(Node_t* node)
{
    switch(node->type)
    {
        case CONST:
            return _NUM(0);

        case VAR:
            return _NUM(1);

        case OP:
            return DiffOp(node);

        default:
            return NULL;
    }
}


static Node_t* DiffOp(Node_t* node)
{
    switch(node->value.value_op)
    {
        case ADD:
            return _ADD(DiffNode(node->left), DiffNode(node->right));

        case SUB:
            return _SUB(DiffNode(node->left), DiffNode(node->right));

        case MUL:
        {
            return  _ADD(
                        _MUL(DiffNode(node->left), TreeCpy(node->right)),
                        _MUL(TreeCpy(node->left), DiffNode(node->right))
                        );
        }

        case DIV:
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
        }

        case EXP:
            return _MUL(_EXP(TreeCpy(node->left)), DiffNode(node->left));

        case LN:
            return _DIV(DiffNode(node->left), TreeCpy(node->left));

        case POW:
        {
            bool var_in_base  = CheckTreeForVars(node->left);
            bool var_in_power = CheckTreeForVars(node->right);

            if (var_in_base && var_in_power)
            {
                return  _MUL(
                            TreeCpy(node),
                            _ADD(
                                _MUL(
                                    DiffNode(node->right),
                                    _LN(TreeCpy(node->left))
                                    ),
                                _MUL(
                                    TreeCpy(node->right),
                                    DiffNode(_LN(TreeCpy(node->left)))
                                    )
                                )
                            );
            }

            else if (var_in_base)
            {
                return  _MUL(
                            DiffNode(node->left),
                            _MUL(
                                TreeCpy(node->right),
                                _POW(
                                    TreeCpy(node->left),
                                    _SUB(
                                        TreeCpy(node->right),
                                        _NUM(1)
                                        )
                                    )
                                )
                            );
            }

            else if (var_in_power)
            {
                return  _MUL(
                            DiffNode(node->right),
                            _MUL(
                                _LN(TreeCpy(node->left)),
                                _POW(
                                    TreeCpy(node->left),
                                    TreeCpy(node->right)
                                    )
                                )
                            );
            }

            break;
        }

        default:
            return NULL;
    }

    return NULL;
}
