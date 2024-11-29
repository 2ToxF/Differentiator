#ifndef _FORMULA_SCANNER_H
#define _FORMULA_SCANNER_H

#include "tree.h"

Node_t* ScanFormulaFromFile(const char* file_name, CodeError* p_code_err);

#endif
