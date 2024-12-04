#ifndef _TREE_H
#define _TREE_H

#include <stdio.h>

/// -----------------------------------------------------------------------------------------------------------

/// Creates node with constant
#define _NUM(__number__) NewNodeConst(__number__, NULL, NULL)

/// Create node with variables
#define _X               NewNodeVar('x',       NULL, NULL)
#define _Y               NewNodeVar('y',       NULL, NULL)
#define _Z               NewNodeVar('z',       NULL, NULL)
#define _VAR(__char__)   NewNodeVar(__char__,  NULL, NULL)

/// Create node with operations (or functions)
#define _ADD(__left_son__, __right_son__) NewNodeOp(ADD, __left_son__, __right_son__)  // Вопрос: можно ли убрать копипаст тут?
#define _SUB(__left_son__, __right_son__) NewNodeOp(SUB, __left_son__, __right_son__)
#define _MUL(__left_son__, __right_son__) NewNodeOp(MUL, __left_son__, __right_son__)
#define _DIV(__left_son__, __right_son__) NewNodeOp(DIV, __left_son__, __right_son__)
#define _POW(__left_son__, __right_son__) NewNodeOp(POW, __left_son__, __right_son__)

#define _EXP(__left_son__)                NewNodeOp(EXP, __left_son__, NULL)
#define _LN(__left_son__)                 NewNodeOp(LN,  __left_son__, NULL)
#define _LOG(__left_son__, __right_son__) NewNodeOp(LOG, __left_son__, __right_son__)

#define _SIN(__left_son__) NewNodeOp(SIN, __left_son__, NULL)
#define _COS(__left_son__) NewNodeOp(COS, __left_son__, NULL)
#define _TAN(__left_son__) NewNodeOp(TAN, __left_son__, NULL)
#define _CTG(__left_son__) NewNodeOp(CTG, __left_son__, NULL)

#define _SH(__left_son__)  NewNodeOp(SH, __left_son__,  NULL)
#define _CH(__left_son__)  NewNodeOp(CH, __left_son__,  NULL)
#define _TH(__left_son__)  NewNodeOp(TH, __left_son__,  NULL)
#define _CTH(__left_son__) NewNodeOp(CTH, __left_son__, NULL)

#define _ARCSIN(__left_son__) NewNodeOp(ARCSIN, __left_son__, NULL)
#define _ARCCOS(__left_son__) NewNodeOp(ARCCOS, __left_son__, NULL)
#define _ARCTAN(__left_son__) NewNodeOp(ARCTAN, __left_son__, NULL)
#define _ARCCTG(__left_son__) NewNodeOp(ARCCTG, __left_son__, NULL)

#define _ARCSH(__left_son__)  NewNodeOp(ARCSH, __left_son__,  NULL)
#define _ARCCH(__left_son__)  NewNodeOp(ARCCH, __left_son__,  NULL)
#define _ARCTH(__left_son__)  NewNodeOp(ARCTH, __left_son__,  NULL)
#define _ARCCTH(__left_son__) NewNodeOp(ARCCTH, __left_son__, NULL)

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
    double             value_const;
    OperationsAndFuncs value_op;
    char               value_var;
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
Node_t* NewNodeConst     (double             elem_value, Node_t* left_son_ptr, Node_t* right_son_ptr);
Node_t* NewNodeOp        (OperationsAndFuncs elem_value, Node_t* left_son_ptr, Node_t* right_son_ptr);
Node_t* NewNodeVar       (char               elem_value, Node_t* left_son_ptr, Node_t* right_son_ptr);
void    SimplifyTree     (Node_t* node, CodeError* p_code_err);
Node_t* TreeCpy          (Node_t* node_src);
void    TreeDtor         (Node_t* node);


#endif
