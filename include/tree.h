#ifndef _TREE_H
#define _TREE_H

#include <stdio.h>

/// -----------------------------------------------------------------------------------------------------------

/// Creates node with constant
#define _NUM(__number__) NewNode(CONST, __number__, NULL, NULL)

/// Create node with common variables
#define _X               NewNode(VAR,   (int) 'x',  NULL, NULL)
#define _Y               NewNode(VAR,   (int) 'y',  NULL, NULL)
#define _Z               NewNode(VAR,   (int) 'z',  NULL, NULL)

/// Create node with operations (and functions)
#define _ADD(__left_son__, __right_son__) NewNode(OP, ADD, __left_son__, __right_son__)
#define _SUB(__left_son__, __right_son__) NewNode(OP, SUB, __left_son__, __right_son__)
#define _MUL(__left_son__, __right_son__) NewNode(OP, MUL, __left_son__, __right_son__)
#define _DIV(__left_son__, __right_son__) NewNode(OP, DIV, __left_son__, __right_son__)

#define _EXP(__left_son__)                NewNode(OP, EXP, __left_son__, NULL)
#define _LN(__left_son__)                 NewNode(OP, LN,  __left_son__, NULL)

#define _POW(__left_son__, __right_son__) NewNode(OP, POW, __left_son__, __right_son__)
#define _LOG(__left_son__, __right_son__) NewNode(OP, LOG, __left_son__, __right_son__)

#define _SIN(__left_son__) NewNode(OP, SIN, __left_son__, NULL)
#define _COS(__left_son__) NewNode(OP, COS, __left_son__, NULL)
#define _TAN(__left_son__) NewNode(OP, TAN, __left_son__, NULL)
#define _CTG(__left_son__) NewNode(OP, CTG, __left_son__, NULL)

#define _SH(__left_son__)  NewNode(OP, SH, __left_son__,  NULL)
#define _CH(__left_son__)  NewNode(OP, CH, __left_son__,  NULL)
#define _TH(__left_son__)  NewNode(OP, TH, __left_son__,  NULL)
#define _CTH(__left_son__) NewNode(OP, CTH, __left_son__, NULL)

#define _ARCSIN(__left_son__) NewNode(OP, ARCSIN, __left_son__, NULL)
#define _ARCCOS(__left_son__) NewNode(OP, ARCCOS, __left_son__, NULL)
#define _ARCTAN(__left_son__) NewNode(OP, ARCTAN, __left_son__, NULL)
#define _ARCCTG(__left_son__) NewNode(OP, ARCCTG, __left_son__, NULL)

#define _ARCSH(__left_son__)  NewNode(OP, ARCSH, __left_son__,  NULL)
#define _ARCCH(__left_son__)  NewNode(OP, ARCCH, __left_son__,  NULL)
#define _ARCTH(__left_son__)  NewNode(OP, ARCTH, __left_son__,  NULL)
#define _ARCCTH(__left_son__) NewNode(OP, ARCCTH, __left_son__, NULL)

/// -----------------------------------------------------------------------------------------------------------

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

    EXP, LN,
    POW, LOG,

    SIN, COS,
    TAN, CTG,

    SH, CH,
    TH, CTH,

    ARCSIN, ARCCOS,
    ARCTAN, ARCCTG,

    ARCSH, ARCCH,
    ARCTH, ARCCTH,
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

/// -----------------------------------------------------------------------------------------------------------

Node_t* NewNode     (TreeElemType elem_type, double elem_value,
                     Node_t* left_son_ptr, Node_t* right_son_ptr);
bool    SimplifyTree(Node_t* node, CodeError* p_code_err);
Node_t* TreeCpy     (Node_t* node_src);
void    TreeDtor    (Node_t* node);


#endif
