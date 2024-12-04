#include "settings.h" // !!!

#include <stdlib.h>

#include "differentiator.h"
// #include "tree_dump.h"
#include "tree.h"

static Node_t* DiffOp  (Node_t* node);
static Node_t* DiffPow (Node_t* node);


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


#define DEF_OP(__op_name__, __code_for_diff__, ...)                 \
    case __op_name__:                                               \
        {__code_for_diff__}                                         \
        break;

#define DEF_OP_ONE_ARG(__op_name__, __code_for_diff__, ...)         \
    DEF_OP(__op_name__, __code_for_diff__, __VA_ARGS__)

static Node_t* DiffOp(Node_t* node)
{
    switch(node->value.value_op)
    {
        #include "opers_and_funcs.h"

        default:
            return NULL;
    }

    return NULL;
}

#undef DEF_OP


static Node_t* DiffPow(Node_t* node)
{
    bool var_in_base  = CheckTreeForVars(node->left);
    bool var_in_power = CheckTreeForVars(node->right);

    if (var_in_base && var_in_power)
    {
        Node_t* new_part_of_node = _LN(TreeCpy(node->left));

        Node_t* new_diff_node = _MUL(
                                    TreeCpy(node),
                                    _ADD(
                                        _MUL(
                                            DiffNode(node->right),
                                            new_part_of_node
                                            ),
                                        _MUL(
                                            TreeCpy(node->right),
                                            DiffNode(new_part_of_node)
                                            )
                                        )
                                    );

        free(new_part_of_node); new_part_of_node = NULL;
        return new_diff_node;
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

    return NULL;
}
