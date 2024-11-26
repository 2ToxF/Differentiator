#include "settings.h" // !!!

#include <stdlib.h>

#include "tree.h"
#include "tree_dump.h"
#include "utils.h"

static bool DoOpWithTwoConsts(Node_t* node, CodeError* p_code_err);
static void NodeCpy          (Node_t* node_dest, Node_t* node_src);


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


static void NodeCpy(Node_t* node_dest, Node_t* node_src)
{
    node_dest->type  = node_src->type;
    node_dest->value = node_src->value;

    Node_t* left_son_ptr = node_dest->left;
    node_dest->left = node_src->left;
    free(left_son_ptr);

    Node_t* right_son_ptr = node_dest->right;
    node_dest->right = node_src->right;
    free(right_son_ptr);
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


#define DO_OP_WITH_ONE_CONST_(__const_node__, __nonconst_node__)                                            \
    if (node->value.value_op == MUL && IsZero(node->__const_node__->value.value_const))                     \
    {                                                                                                       \
        node->type = CONST;                                                                                 \
        node->value.value_const = 0;                                                                        \
                                                                                                            \
        free(node->left); node->left = NULL;                                                                \
        free(node->right); node->right = NULL;                                                              \
    }                                                                                                       \
                                                                                                            \
    else if ((node->value.value_op == MUL || node->value.value_op == DIV) &&                                \
             IsEqual(node->__const_node__->value.value_const, 1))                                           \
    {                                                                                                       \
        NodeCpy(node, node->__nonconst_node__);                                                             \
    }                                                                                                       \
                                                                                                            \
    else if (node->value.value_op == DIV && IsZero(node->__const_node__->value.value_const))                \
        *p_code_err = ZERO_DIVISION_ERR;                                                                    \
                                                                                                            \
    else if ((node->value.value_op == ADD || node->value.value_op == SUB) &&                                \
                IsZero(node->__const_node__->value.value_const))                                            \
    {                                                                                                       \
        NodeCpy(node, node->__nonconst_node__);                                                             \
    }

bool SimplifyTree(Node_t* node, CodeError* p_code_err)
{
    if (node == NULL || *p_code_err != NO_ERR)
        return false;

    while (SimplifyTree(node->left, p_code_err))  {}
    while (SimplifyTree(node->right, p_code_err)) {}

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


void TreeDtor(Node_t* node)
{
    if (node->left != NULL)
        TreeDtor(node->left);
    if (node->right != NULL)
        TreeDtor(node->right);

    free(node);
}
