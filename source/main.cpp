#include "settings.h" // !!!

#include "differentiator.h"
#include "formula_scanner.h"
#include "input_output.h"
#include "tree_dump.h"
#include "tree.h"

static const char* const INPUT_FILE_WITH_FORMULAS = FORMULAS_DIR "formulas.txt";

CodeError RunProgram();

int main()
{
    CodeError code_err = RunProgram();
    PrintCodeError(code_err);
    return code_err;
}


#define CHECK_AND_CALL_ERR_         \
    if (code_err != NO_ERR)         \
        return code_err

CodeError RunProgram()
{
    CodeError code_err = NO_ERR;

    Node_t* root = ScanFormulaFromFile(INPUT_FILE_WITH_FORMULAS, &code_err);
    CHECK_AND_CALL_ERR_;
    TreeDump(root);

    SimplifyTree(root, &code_err);
    CHECK_AND_CALL_ERR_;
    TreeDump(root);

    Node_t* diff_tree = DiffNode(root);
    TreeDump(diff_tree);

    SimplifyTree(diff_tree, &code_err);
    CHECK_AND_CALL_ERR_;
    TreeDump(diff_tree);

    DumpClose();

    // TexTree(root);

    return code_err;
}
