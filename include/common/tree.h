#ifndef _TREE_H
#define _TREE_H

#include <stdio.h>

/// -----------------------------------------------------------------------------------------------------------

/// Creates node with constant
#define _NUM(__number__) NewNode(CONST, __number__, NULL, NULL)

/// Create node with common variables
#define _X               NewNode(VAR,   (int) 'x',       NULL, NULL) // TODO: wtf
#define _Y               NewNode(VAR,   (int) 'y',       NULL, NULL)
#define _Z               NewNode(VAR,   (int) 'z',       NULL, NULL)
#define _VAR(__char__)   NewNode(VAR,   (int) __char__,  NULL, NULL)

/// Create node with operations (and functions)
#define _ADD(__left_son__, __right_son__) NewNode(OP, ADD, __left_son__, __right_son__)  // Вопрос: можно ли убрать копипаст тут?
#define _SUB(__left_son__, __right_son__) NewNode(OP, SUB, __left_son__, __right_son__)
#define _MUL(__left_son__, __right_son__) NewNode(OP, MUL, __left_son__, __right_son__)
#define _DIV(__left_son__, __right_son__) NewNode(OP, DIV, __left_son__, __right_son__)
#define _POW(__left_son__, __right_son__) NewNode(OP, POW, __left_son__, __right_son__)

#define _EXP(__left_son__)                NewNode(OP, EXP, __left_son__, NULL)
#define _LN(__left_son__)                 NewNode(OP, LN,  __left_son__, NULL)
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


#define DEF_OP(__op_name__, ...) \
    __op_name__,
#define DEF_OP_ONE_ARG(__op_name__, ...) DEF_OP(__op_name__)

enum OperationsAndFuncs
{
    #include "opers_and_funcs.h"
};

#undef DEF_OP
#undef DEF_OP_ONE_ARG

/// -----------------------------------------------------------------------------------------------------------

union TreeElemUnion
{
    double value_const;
    char   value_var;
    char   value_op;
};

struct Node_t
{
    TreeElemType type;
    TreeElemUnion value;

    Node_t* left;
    Node_t* right;
};

/// -----------------------------------------------------------------------------------------------------------

bool    CheckTreeForVars (Node_t* node);
Node_t* NewNode          (TreeElemType elem_type, double elem_value,
                          Node_t* left_son_ptr, Node_t* right_son_ptr);  // Вопрос: можно ли передавать через union, а не double?
void    SimplifyTree     (Node_t* node, CodeError* p_code_err);
Node_t* TreeCpy          (Node_t* node_src);
void    TreeDtor         (Node_t* node);


#endif
