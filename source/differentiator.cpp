#include "settings.h" // !!!

#include <stdlib.h>

#include "differentiator.h"
#include "tree.h"

static Node_t* DiffOp  (Node_t* node, char var_diff_by);
static Node_t* DiffPow (Node_t* node, char var_diff_by);


Node_t* DiffNode(Node_t* node, char var_diff_by)
{
    if (!CheckTreeForVar(node, var_diff_by))
        return _NUM(0);

    switch(node->type)
    {
        case CONST:
            return _NUM(0);

        case VAR:
            return _NUM(1);

        case OP:
            return DiffOp(node, var_diff_by);

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

static Node_t* DiffOp(Node_t* node, char var_diff_by)
{
    if (!CheckTreeForVar(node, var_diff_by))
        return TreeCpy(node);

    switch(node->value.value_op)
    {
        #include "opers_and_funcs.h"

        default:
            return NULL;
    }

    return NULL;
}

#undef DEF_OP


static Node_t* DiffPow(Node_t* node, char var_diff_by)
{
    bool var_in_base  = CheckTreeForVar(node->left, var_diff_by);
    bool var_in_power = CheckTreeForVar(node->right, var_diff_by);

    if (var_in_base && var_in_power)
    {
        Node_t* new_part_of_node = _LN(TreeCpy(node->left));

        Node_t* new_diff_node = _MUL(
                                    TreeCpy(node),
                                    _ADD(
                                        _MUL(
                                            DiffNode(node->right, var_diff_by),
                                            new_part_of_node
                                            ),
                                        _MUL(
                                            TreeCpy(node->right),
                                            DiffNode(new_part_of_node, var_diff_by)
                                            )
                                        )
                                    );

        free(new_part_of_node); new_part_of_node = NULL;
        return new_diff_node;
    }

    else if (var_in_base)
    {
        return  _MUL(
                    DiffNode(node->left, var_diff_by),
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
                    DiffNode(node->right, var_diff_by),
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
