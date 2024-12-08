#ifndef _TREE_TEX_H
#define _TREE_TEX_H

#include "tree.h"

CodeError MakeCommonPdf   ();
CodeError MakeSpecialPdf  ();
void      TexTreeEquation (FILE*  tex_file_ptr, Node_t* node);

#endif
