#include "settings.h" // !!!

#include <math.h>
#include <stdlib.h>

#include "differentiator.h"
#include "tex_pdf.h"
#include "tree_dump.h"
#include "tree.h"
#include "utils.h"

static bool      DoOpWithTwoConsts (Node_t* node, CodeError* p_code_err);
static void      NodeCpy           (Node_t* node_dest, Node_t* node_src);
static void      NullifyNode       (Node_t* node);
static void      SetNodeConstValue (Node_t* node, double new_value);
static bool      SimplifyConstOne  (Node_t* node, CodeError* p_code_err);
static bool      SimplifyConstZero (Node_t* node, CodeError* p_code_err);


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
            {
                *p_code_err = ZERO_DIVISION_ERR;
                return false;
            }

            node->value.value_const = node->left->value.value_const / node->right->value.value_const;
            break;
        }

        case POW:
        {
            if (node->left->value.value_const <= 0)
            {
                *p_code_err = ZERO_TO_NONPOS_POWER_ERR;
                return false;
            }

            node->value.value_const = pow(node->left->value.value_const, node->right->value.value_const);
            break;
        }

        default:
            return false;
    }

    node->type = CONST;
    TreeDtor(node->left); node->left = NULL;
    TreeDtor(node->right); node->right = NULL;

    return true;
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


static bool SimplifyConstOne(Node_t* node, CodeError* /*p_code_err*/)
{
    if (node->left->type == CONST)
    {
        if (node->value.value_op == MUL)
            NodeCpy(node, node->right);

        else if (node->value.value_op == POW)
            SetNodeConstValue(node, 1);

        else
            return false;
    }

    else if (node->right->type == CONST)
    {
        if (node->value.value_op == MUL)
            NodeCpy(node, node->left);

        else if (node->value.value_op == DIV)
            NodeCpy(node, node->left);

        else if (node->value.value_op == POW)
            NodeCpy(node, node->left);

        else
            return false;
    }

    else
        return false;

    return true;
}


static bool SimplifyConstZero(Node_t* node, CodeError* p_code_err)
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
            {
                *p_code_err = ZERO_TO_NONPOS_POWER_ERR;
                return false;
            }

            else
                NullifyNode(node);
        }

        else
            return false;
    }

    else if (node->right->type == CONST)
    {
        if (node->value.value_op == ADD)
            NodeCpy(node, node->left);

        else if (node->value.value_op == SUB)
            NodeCpy(node, node->left);

        else if (node->value.value_op == DIV)
        {
            *p_code_err = ZERO_DIVISION_ERR;
            return false;
        }

        else if (node->value.value_op == POW)
            SetNodeConstValue(node, 1);

        else
            return false;
    }

    else
        return false;

    return true;
}


void SimplifyTree(Node_t* root, Node_t* node, SimplifyType simplify_type,
                  FILE* tex_file_ptr, CodeError* p_code_err)
{
    if (node == NULL)
        return;

    SimplifyTree(root, node->left, simplify_type, tex_file_ptr, p_code_err);
    if (*p_code_err != NO_ERR)
        return;

    SimplifyTree(root, node->right, simplify_type, tex_file_ptr, p_code_err);
    if (*p_code_err != NO_ERR)
        return;

    bool is_node_changed = false;

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
                    is_node_changed = DoOpWithTwoConsts(node, p_code_err);

                else if (node->left->type == CONST)
                {
                    if (IsZero(node->left->value.value_const))
                        is_node_changed = SimplifyConstZero(node, p_code_err);

                    else if (IsEqual(node->left->value.value_const, 1))
                        is_node_changed = SimplifyConstOne(node, p_code_err);
                }

                else if (node->right->type == CONST)
                {
                    if (IsZero(node->right->value.value_const))
                        is_node_changed = SimplifyConstZero(node, p_code_err);

                    else if (IsEqual(node->right->value.value_const, 1))
                        is_node_changed = SimplifyConstOne(node, p_code_err);
                }
            }

            break;
        }

        default:
            *p_code_err = UNKNOWN_NODE_VALUE_TYPE_ERR;
            return;
    }

    if (simplify_type == WITH_TEX && is_node_changed)
    {
        if (tex_file_ptr == NULL)
            *p_code_err = PRINT_TO_NULL_PTR_ERR;

        else
            TexTreeEquation(tex_file_ptr, root);
    }
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
