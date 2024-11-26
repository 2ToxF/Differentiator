#include "settings.h" // !!!

#include "tree.h"
#include "tree_dump.h"
#include "input_output.h"

static const char* const FILE_WITH_FORMULAS = TEX_DIR "formulas.txt";

CodeError RunProgram();

int main()
{
    CodeError code_err = RunProgram();
    PrintCodeError(code_err);
    return code_err;
}


CodeError RunProgram()
{
    CodeError code_err = NO_ERR;

    Node_t* root = ScanTreeFromFile(FILE_WITH_FORMULAS, &code_err);
    TreeDump(root);
    return NO_ERR;
}
