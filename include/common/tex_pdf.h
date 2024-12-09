#ifndef _TREE_TEX_H
#define _TREE_TEX_H

#include "tree.h"

CodeError MakeCommonPdf   (char var_diff_by);
CodeError MakeSpecialPdf  (char var_diff_by);
void      TexTreeEquation (FILE* tex_file_ptr, Node_t* node);

#endif
