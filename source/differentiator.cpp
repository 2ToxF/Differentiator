#include "settings.h" // !!!

#include "differentiator.h"
#include "tree.h"

static Node_t* DiffOp(Node_t* node);


Node_t* DiffNode(Node_t* node)
{
    switch(node->type)
    {
        case CONST:
            return _NUM(0);

        case VAR:
            return _NUM(1);

        case OP:
            return DiffOp(node);

        default:
            return NULL;
    }
}


#define DIFF_OF_COMPOSITION_(__result_op__)                             \
    _MUL(_##__result_op__(TreeCpy(node->left)), DiffNode(node->left))   \

static Node_t* DiffOp(Node_t* node)
{
    switch(node->value.value_op)
    {
        case ADD:
            return _ADD(DiffNode(node->left), DiffNode(node->right));

        case SUB:
            return _SUB(DiffNode(node->left), DiffNode(node->right));

        case MUL:
        {
            return  _ADD
                        (
                        _MUL(DiffNode(node->left), TreeCpy(node->right)),
                        _MUL(TreeCpy(node->left), DiffNode(node->right))
                        );
        }

        case DIV:
        {
            return  _DIV
                        (
                        _SUB
                            (
                            _MUL(DiffNode(node->left), TreeCpy(node->right)),
                            _MUL(TreeCpy(node->left), DiffNode(node->right))
                            ),
                        _POW
                            (
                            TreeCpy(node->right),
                            _NUM(2)
                            )
                        );
        }

        case EXP:
            return DIFF_OF_COMPOSITION_(EXP);

        case LN:
            return _MUL(DiffNode(node->left), _DIV(_NUM(1), TreeCpy(node->left)));

        case POW: /* РЫДАТЬ */

        default:
            return NULL;
    }
}

#undef DIFF_OF_COMPOSITION_
