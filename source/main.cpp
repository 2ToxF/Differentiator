#include "settings.h" // !!!

#include "tree.h"
#include "tree_dump.h"

int main()
{
    CodeError code_err = NO_ERR;

    Node_t* root = _DIV(
                        _ADD(
                             _ADD(
                                  _MUL(_NUM(0), _Y),
                                  _MUL(_NUM(1), _X)
                                 ),
                             _MUL(_NUM(2), _Z)
                            ),
                        _ADD(
                             _ADD(
                                  _MUL(_NUM(0), _Z),
                                  _MUL(_NUM(0), _X)
                                 ),
                             _NUM(1)
                            )
                       );
    TreeDump(root);

    SimplifyTree(root, &code_err);
    TreeDump(root);

    DumpClose();

    TreeDtor(root);
    return NO_ERR;
}
