#include "settings.h" // !!!

#include <stdlib.h>

#include "tree.h"
#include "utils.h"

static bool DoOpWithTwoConsts(Node_t* node, CodeError* p_code_err);


static bool DoOpWithTwoConsts(Node_t* node, CodeError* p_code_err)
{
    node->type = CONST;

    switch(node->value.value_op)
    {
        case ADD:
            node->value.value_const = node->left->value.value_const + node->right->value.value_const;
            break;

        case SUB:
            node->value.value_const = node->left->value.value_const - node->right->value.value_const;
            break;

        case DIV:
        {
            if (IsZero(node->right->value.value_const))
            {
                *p_code_err = ZERO_DIVISION_ERR;
                return false;
            }

            node->value.value_const = node->left->value.value_const / node->right->value.value_const;
            break;
        }

        case MUL:
            node->value.value_const = node->left->value.value_const * node->right->value.value_const;
            break;

        default:
            return true;
    }

    free(node->left); node->left = NULL;
    free(node->right); node->right = NULL;
    return false;
}


Node_t* NewNode(TreeElemType elem_type, double elem_value, Node_t* left_son_ptr, Node_t* right_son_ptr)
{
    Node_t* new_node = (Node_t*) calloc(1, sizeof(Node_t));

    new_node->type  = elem_type;

    switch(elem_type)
    {
        case CONST:
            new_node->value.value_const = (double) elem_value;
            break;

        case VAR:
            new_node->value.value_var = (char) elem_value;
            break;

        case OP:
            new_node->value.value_op = (char) elem_value;
            break;

        default:
            return NULL;
    }

    new_node->left  = left_son_ptr;
    new_node->right = right_son_ptr;

    return new_node;
}


#define DO_OP_WITH_ONE_CONST_(__const_node__, __nonconst_node__)                                             \
    if (node->value.value_op == MUL && IsZero(node->__const_node__->value.value_const))                      \
    {                                                                                                        \
        node->type = CONST;                                                                                  \
        node->value.value_const = 0;                                                                         \
                                                                                                             \
        free(node->left); node->left = NULL;                                                                 \
        free(node->right); node->right = NULL;                                                               \
    }                                                                                                        \
                                                                                                             \
    else if ((node->value.value_op == MUL || node->value.value_op == DIV) &&                                 \
             IsEqual(node->__const_node__->value.value_const, 1))                                            \
    {                                                                                                        \
        node->type  = node->__nonconst_node__->type;                                                         \
        node->value = node->__nonconst_node__->value;                                                        \
                                                                                                             \
        free(node->left); node->left = NULL;                                                                 \
        free(node->right); node->right = NULL;                                                               \
    }                                                                                                        \
                                                                                                             \
    else if (node->value.value_op == DIV && IsZero(node->__const_node__->value.value_const))                 \
    {                                                                                                        \
        *p_code_err = ZERO_DIVISION_ERR;                                                                     \
        return false;                                                                                        \
    }                                                                                                        \
                                                                                                             \
    else if ((node->value.value_op == ADD || node->value.value_op == SUB) &&                                 \
                IsZero(node->__const_node__->value.value_const))                                             \
    {                                                                                                        \
        node->type  = node->__nonconst_node__->type;                                                         \
        node->value = node->__nonconst_node__->value;                                                        \
                                                                                                             \
        free(node->left); node->left = NULL;                                                                 \
        free(node->right); node->right = NULL;                                                               \
    }

bool SimplifyTree(Node_t* node, CodeError* p_code_err)
{
    if (node == NULL || *p_code_err != NO_ERR)
        return false;

    while (SimplifyTree(node->left, p_code_err))  {printf("left = %d\n", node->value.value_op);}
    while (SimplifyTree(node->right, p_code_err)) {printf("right = %d\n", node->value.value_op);}

    switch (node->type)
    {
        case CONST:
            return false;

        case VAR:
            return false;

        case OP:
        {
            if (node->left != NULL && node->right != NULL)
            {
                if (node->left->type == CONST && node->right->type == CONST)
                {
                    return DoOpWithTwoConsts(node, p_code_err);
                }

                else if (node->left->type == CONST)
                {
                    DO_OP_WITH_ONE_CONST_(left, right);
                }

                else if (node->right->type == CONST)
                {
                    DO_OP_WITH_ONE_CONST_(right, left);
                }

                else
                    return true;

                return false;
            }

            break;
        }

        default:
            *p_code_err = UNKNOWN_NODE_VALUE_TYPE_ERR;
            return false;
    }

    return false;
}

#undef DO_OP_WITH_ONE_CONST_


// void TreeInit(TreeHeader* tree_header, Node_t* root)
// {
//     tree_header->tree_code_err = NO_ERR;
//     tree_header->root = root;
//     number_of_nodes = 1;
// }


void TreeDtor(Node_t* node)
{
    if (node->left != NULL)
        TreeDtor(node->left);
    if (node->right != NULL)
        TreeDtor(node->right);

    free(node);
}
