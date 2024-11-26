#ifndef _TREE_H
#define _TREE_H

#include <stdio.h>

#define _NUM(__number__) NewNode(CONST, __number__, NULL, NULL)
#define _X               NewNode(VAR,   (int) 'x',  NULL, NULL)

#define _ADD(__left_son__, __right_son__) NewNode(OP, ADD, __left_son__, __right_son__)
#define _SUB(__left_son__, __right_son__) NewNode(OP, SUB, __left_son__, __right_son__)
#define _MUL(__left_son__, __right_son__) NewNode(OP, MUL, __left_son__, __right_son__)
#define _DIV(__left_son__, __right_son__) NewNode(OP, DIV, __left_son__, __right_son__)

enum NodesRelation
{
    NO_RELATION,
    LEFT_SON,
    RIGHT_SON,
};

enum TreeElemType
{
    CONST,
    VAR,
    OP,
};

enum OpType
{
    ADD,
    SUB,
    MUL,
    DIV,

    POW,
    LOG,

    SIN, SH,
    COS, CH,
    TAN, TH,
    CTG, CTH,

    ARCSIN, ARCSH,
    ARCCOS, ARCCH,
    ARCTAN, ARCTH,
    ARCCTG, ARCCTH,
};

enum PowType
{
    VAR_IN_BASE,
    VAR_IN_POW,
    VAR_IN_BOTH,
};

typedef union
{
    double value_const;
    char   value_var;
    char   value_op;
} TreeElemUnion;

struct Node_t
{
    TreeElemType type;
    TreeElemUnion value;

    Node_t* left;
    Node_t* right;
};

// struct TreeHeader
// {
//     CodeError tree_code_err;
//     Node_t*   root;
//     int       number_of_nodes;
// };


Node_t* NewNode     (TreeElemType elem_type, double elem_value,
                     Node_t* left_son_ptr, Node_t* right_son_ptr);
bool    SimplifyTree(Node_t* node, CodeError* p_code_err);
// void    TreeInit    (TreeHeader* tree_header);
void    TreeDtor    (Node_t* node);

#endif
