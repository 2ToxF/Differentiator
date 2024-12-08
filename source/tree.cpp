#include "settings.h" // !!!

#include <math.h>
#include <stdlib.h>

#include "differentiator.h"
#include "tree_dump.h"
#include "tree.h"
#include "utils.h"

static CodeError DoOpWithTwoConsts (Node_t* node);
static void      NodeCpy           (Node_t* node_dest, Node_t* node_src);
static void      NullifyNode       (Node_t* node);
static void      SetNodeConstValue (Node_t* node, double new_value);
static CodeError SimplifyConstOne  (Node_t* node);
static CodeError SimplifyConstZero (Node_t* node);


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


static CodeError DoOpWithTwoConsts(Node_t* node)
{
    switch((char) node->value.value_op)
    {
        case ADD:
            node->value.value_const = node->left->value.value_const + node->right->value.value_const;
            break;

        case SUB:
            node->value.value_const = node->left->value.value_const - node->right->value.value_const;
            break;

        case MUL:
            node->value.value_const = node->left->value.value_const * node->right->value.value_const;
            break;

        case DIV:
        {
            if (IsZero(node->right->value.value_const))
                return ZERO_DIVISION_ERR;

            node->value.value_const = node->left->value.value_const / node->right->value.value_const;
            break;
        }

        case POW:
        {
            if (node->left->value.value_const <= 0)
                return ZERO_TO_NONPOS_POWER_ERR;

            node->value.value_const = pow(node->left->value.value_const, node->right->value.value_const);
            break;
        }

        default:
            return NO_ERR;
    }

    node->type = CONST;
    TreeDtor(node->left); node->left = NULL;
    TreeDtor(node->right); node->right = NULL;

    return NO_ERR;
}


Node_t* NewNodeConst(double elem_value, Node_t* left_son_ptr, Node_t* right_son_ptr)
{
    Node_t* new_node = (Node_t*) calloc(1, sizeof(Node_t));

    new_node->type  = CONST;
    new_node->left  = left_son_ptr;
    new_node->right = right_son_ptr;

    new_node->value.value_const = elem_value;

    return new_node;
}


Node_t* NewNodeOp(OperationsAndFuncs elem_value, Node_t* left_son_ptr, Node_t* right_son_ptr)
{
    Node_t* new_node = (Node_t*) calloc(1, sizeof(Node_t));

    new_node->type  = OP;
    new_node->left  = left_son_ptr;
    new_node->right = right_son_ptr;

    new_node->value.value_op = elem_value;

    return new_node;
}


Node_t* NewNodeVar(char elem_value, Node_t* left_son_ptr, Node_t* right_son_ptr)
{
    Node_t* new_node = (Node_t*) calloc(1, sizeof(Node_t));

    new_node->type  = VAR;
    new_node->left  = left_son_ptr;
    new_node->right = right_son_ptr;

    new_node->value.value_var = elem_value;

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


static CodeError SimplifyConstOne(Node_t* node)
{
    if (node->left->type == CONST)
    {
        if (node->value.value_op == MUL)
            NodeCpy(node, node->right);

        else if (node->value.value_op == POW)
            SetNodeConstValue(node, 1);
    }

    else if (node->right->type == CONST)
    {
        if (node->value.value_op == MUL)
            NodeCpy(node, node->left);

        else if (node->value.value_op == DIV)
            NodeCpy(node, node->left);

        else if (node->value.value_op == POW)
            NodeCpy(node, node->left);
    }

    return NO_ERR;
}


static CodeError SimplifyConstZero(Node_t* node)
{
    if (node->value.value_op == MUL)
        NullifyNode(node);

    else if (node->left->type == CONST)
    {
        if (node->value.value_op == ADD)
            NodeCpy(node, node->right);

        else if (node->value.value_op == SUB)
        {
            node->value.value_op          = MUL;
            node->left->value.value_const = -1;
        }

        else if (node->value.value_op == DIV)
            NullifyNode(node);

        else if (node->value.value_op == POW)
        {
            if (node->right->type == CONST && node->right->value.value_const <= 0)
                return ZERO_TO_NONPOS_POWER_ERR;

            else
                NullifyNode(node);
        }
    }

    else if (node->right->type == CONST)
    {
        if (node->value.value_op == ADD)
            NodeCpy(node, node->left);

        else if (node->value.value_op == SUB)
            NodeCpy(node, node->left);

        else if (node->value.value_op == DIV)
            return ZERO_DIVISION_ERR;

        else if (node->value.value_op == POW)
            SetNodeConstValue(node, 1);

    }

    return NO_ERR;
}


CodeError SimplifyTree(Node_t* node)
{
    if (node == NULL)
        return NO_ERR;

    CodeError code_err = SimplifyTree(node->left);
    if (code_err != NO_ERR)
        return code_err;

    code_err = SimplifyTree(node->right);
    if (code_err != NO_ERR)
        return code_err;


    switch (node->type)
    {
        case CONST:
            break;

        case VAR:
            break;

        case OP:
        {
            if (node->left != NULL && node->right != NULL)
            {
                if (node->left->type == CONST && node->right->type == CONST)
                    return DoOpWithTwoConsts(node);

                else if (node->left->type == CONST)
                {
                    if (IsZero(node->left->value.value_const))
                        return SimplifyConstZero(node);

                    else if (IsEqual(node->left->value.value_const, 1))
                        return SimplifyConstOne(node);
                }

                else if (node->right->type == CONST)
                {
                    if (IsZero(node->right->value.value_const))
                        return SimplifyConstZero(node);

                    else if (IsEqual(node->right->value.value_const, 1))
                        return SimplifyConstOne(node);
                }
            }

            break;
        }

        default:
            return UNKNOWN_NODE_VALUE_TYPE_ERR;
    }

    return NO_ERR;
}


Node_t* TreeCpy(Node_t* node_src)
{
    if (node_src == NULL)
        return NULL;

    switch (node_src->type)
    {
        case CONST:
            return NewNodeConst(node_src->value.value_const, TreeCpy(node_src->left), TreeCpy(node_src->right));

        case VAR:
            return NewNodeVar(node_src->value.value_var, TreeCpy(node_src->left), TreeCpy(node_src->right));

        case OP:
            return NewNodeOp(node_src->value.value_op, TreeCpy(node_src->left), TreeCpy(node_src->right));

        default:
            return NULL;
    }
}


void TreeDtor(Node_t* node)
{
    if (node == NULL)
        return;

    TreeDtor(node->left);
    TreeDtor(node->right);

    free(node);
}
