#include "settings.h" // !!!

#include "formula_scanner.h"
#include "input_output.h"
#include "tree.h"
#include "tree_dump.h"

static const char* const INPUT_FILE_WITH_FORMULAS = FORMULAS_DIR "formulas.txt";

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

    Node_t* root = ScanFormulaFromFile(INPUT_FILE_WITH_FORMULAS, &code_err);
    TreeDump(root);

    return code_err;
}
