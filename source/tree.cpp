#include "settings.h" // !!!

#include <stdlib.h>

#include "tree.h"
#include "utils.h"

static bool DoOpWithTwoConsts(Node_t* node, CodeError* p_code_err);
static void NodeCpy          (Node_t* node_dest, Node_t* node_src);
static void NullifyNode      (Node_t* node);
static void SetNodeConstValue(Node_t* node, double new_value);


bool CheckTreeForVars(Node_t* node)
{
    if (node == NULL)
        return false;

    if (node->type == VAR)
        return true;

    if (CheckTreeForVars(node->left) == true)
        return true;

    return CheckTreeForVars(node->right);
}


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

    TreeDtor(node->left); node->left = NULL;
    TreeDtor(node->right); node->right = NULL;
    return false;
}


Node_t* NewNode(TreeElemType node_type, double node_value, Node_t* left_son_ptr, Node_t* right_son_ptr)
{
    Node_t* new_node = (Node_t*) calloc(1, sizeof(Node_t));

    new_node->type  = node_type;

    switch(node_type)
    {
        case CONST:
            new_node->value.value_const = (double) node_value;
            break;

        case VAR:
            new_node->value.value_var = (char) node_value;
            break;

        case OP:
            new_node->value.value_op = (char) node_value;
            break;

        default:
            return NULL;
    }

    new_node->left  = left_son_ptr;
    new_node->right = right_son_ptr;

    return new_node;
}


static void NodeCpy(Node_t* node_dest, Node_t* node_src)
{
    node_dest->type  = node_src->type;
    node_dest->value = node_src->value;

    Node_t* left_son_ptr = node_dest->left;
    node_dest->left = node_src->left;

    if (node_src != left_son_ptr)
        TreeDtor(left_son_ptr);

    Node_t* right_son_ptr = node_dest->right;
    node_dest->right = node_src->right;

    if (node_src != right_son_ptr)
        TreeDtor(right_son_ptr);
}


static void NullifyNode(Node_t* node)
{
    SetNodeConstValue(node, 0);
}


static void SetNodeConstValue(Node_t* node, double new_value)
{
    node->type = CONST;
    node->value.value_const = new_value;

    TreeDtor(node->left); node->left = NULL;
    TreeDtor(node->right); node->right = NULL;
}


#define DO_OP_WITH_ONE_CONST_(__const_node__, __nonconst_node__)                                            \
    else if (node->value.value_op == MUL && IsZero(node->__const_node__->value.value_const))                \
        NullifyNode(node);                                                                                  \
                                                                                                            \
    else if (node->value.value_op == MUL && IsEqual(node->__const_node__->value.value_const, 1))            \
        NodeCpy(node, node->__nonconst_node__);                                                             \
                                                                                                            \
    else if (node->value.value_op == ADD && IsZero(node->__const_node__->value.value_const))                \
        NodeCpy(node, node->__nonconst_node__);

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
                    if (node->value.value_op == SUB && IsZero(node->left->value.value_const))
                    {
                        node->value.value_op          = MUL;
                        node->left->value.value_const = -1;
                    }

                    else if (node->value.value_op == DIV && IsZero(node->left->value.value_const))
                        NullifyNode(node);

                    else if (node->value.value_op == POW && IsEqual(node->left->value.value_const, 1))
                        SetNodeConstValue(node, 1);

                    else if (node->value.value_op == POW && IsZero(node->left->value.value_const))
                    {
                        if (node->right->type == CONST && node->right->value.value_op <= 0)
                        {
                            *p_code_err = ZERO_TO_NONPOS_POWER_ERR;
                            return false;
                        }

                        else
                            NullifyNode(node);
                    }

                    DO_OP_WITH_ONE_CONST_(left, right);
                }

                else if (node->right->type == CONST)
                {
                    if (node->value.value_op == SUB && IsZero(node->right->value.value_const))
                        NodeCpy(node, node->left);

                    else if (node->value.value_op == DIV && IsZero(node->right->value.value_const))
                    {
                        *p_code_err = ZERO_DIVISION_ERR;
                        return false;
                    }

                    else if (node->value.value_op == POW && IsEqual(node->right->value.value_const, 1))
                        NodeCpy(node, node->left);

                    else if (node->value.value_op == POW && IsZero(node->right->value.value_const))
                        SetNodeConstValue(node, 1);

                    else if (node->value.value_op == DIV && IsEqual(node->right->value.value_const, 1))
                        NodeCpy(node, node->left);

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


Node_t* TreeCpy(Node_t* node_src)
{
    if (node_src == NULL)
        return NULL;

    if (node_src->type == CONST)
        return NewNode(node_src->type, node_src->value.value_const,
                       TreeCpy(node_src->left), TreeCpy(node_src->right));

    else
        return NewNode(node_src->type, (double) node_src->value.value_var,
                       TreeCpy(node_src->left), TreeCpy(node_src->right));
}


void TreeDtor(Node_t* node)
{
    if (node->left != NULL)
        TreeDtor(node->left);
    if (node->right != NULL)
        TreeDtor(node->right);

    free(node);
}
