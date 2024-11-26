#include "settings.h" // !!!

#include "tree.h"
#include "tree_dump.h"

int main()
{
    CodeError code_err = NO_ERR;

    Node_t* root = _DIV(_ADD(_X, _NUM(3)), _SUB(_NUM(1000), _NUM(7)));
    TreeDump(root);

    SimplifyTree(root, &code_err);
    TreeDump(root);

    DumpClose();

    TreeDtor(root);
    return NO_ERR;
}
