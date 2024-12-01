#include "settings.h" // !!!

#include <stdlib.h>

#include "differentiator.h"
#include "formula_scanner.h"
#include "input_output.h"
#include "tree_dump.h"
#include "tree_tex.h"
#include "tree.h"

#define CHECK_AND_CALL_ERR_                             \
    if (code_err != NO_ERR)                             \
        return code_err

#define PRINTF_TO_TEX_(__format__, ...)                 \
    fprintf(tex_file_ptr, __format__, ##__VA_ARGS__)

#define TEX_FILES_DIR "tex_files/"
#define TEX_FILE_NAME TEX_FILES_DIR "Differentiator By Antonio Tox(a)icity.tex"

static const char* const INPUT_FILE_WITH_FORMULAS = FORMULAS_DIR "formulas.txt";
static const int         COMMON_FORMULAS_NUMBER   = 24;

FILE* tex_file_ptr = NULL;
// Сохраню пока здесь: pdflatex -output-directory=tex_files -aux-directory=tex_aux_files -quiet "tex_files/Differentiator By Antonio Tox(a)icity.tex"

static CodeError MakeTexWithFormulas();
static CodeError TexEndAndClose     ();
static CodeError TexHeader          ();
static CodeError TexOpen            (const char* file_name);
static void      TexTree            (Node_t* node);


CodeError MakePdfWithFormulas()
{
    CodeError code_err = MakeTexWithFormulas();
    CHECK_AND_CALL_ERR_;

    if (system("pdflatex -output-directory=" TEX_FILES_DIR " -aux-directory=tex_aux_files -quiet "
               "\"" TEX_FILE_NAME "\"") != 0)
        return SYSTEM_CALL_ERR;

    return code_err;
}


static CodeError MakeTexWithFormulas()
{
    CodeError code_err = NO_ERR;

    if (tex_file_ptr == NULL)
    {
        code_err = TexOpen(TEX_FILE_NAME);
        CHECK_AND_CALL_ERR_;

        code_err = TexHeader();
        CHECK_AND_CALL_ERR_;
    }

    fprintf(tex_file_ptr, "Эу, пацан! Пойди сюда...\n"
                          "\n"
                          "Сегодня я хотел бы обсудить с тобой очень важную в математическом анализе тему: \\underline{ПРОИЗВОДНАЯ}. "
                          "Давай начнём с нескольких простых табличных примеров:\n"
                          "\n");

    for (int i = 0; i < COMMON_FORMULAS_NUMBER; ++i)
    {
        Node_t* func_tree = ScanFormulaFromFile(INPUT_FILE_WITH_FORMULAS, &code_err);

        fprintf(tex_file_ptr, "\\begin{equation}\n"
                              "(");

        TexTree(func_tree);
        SimplifyTree(func_tree, &code_err);
        // TreeDump(func_tree);

        fprintf(tex_file_ptr, ")_{x}^{'} = ");

        Node_t* diff_func_tree = DiffNode(func_tree);
        SimplifyTree(diff_func_tree, &code_err);
        // TreeDump(diff_func_tree);
        TexTree(diff_func_tree);

        fprintf(tex_file_ptr, "\n\\end{equation}\n\n");

        // DumpClose();
        // getchar();
    }

    return TexEndAndClose();
}


static CodeError TexEndAndClose()
{
    if (tex_file_ptr == NULL)
        return TEX_TO_NULL_PTR_ERR;

    fprintf(tex_file_ptr, "\n\\end{document}\n");

    fclose(tex_file_ptr); tex_file_ptr = NULL;

    return NO_ERR;
}


static CodeError TexHeader()
{
    CodeError code_err = NO_ERR;

    if (tex_file_ptr == NULL)
    {
        code_err = TexOpen(TEX_FILE_NAME);
        CHECK_AND_CALL_ERR_;
    }

    fprintf(tex_file_ptr, "\\documentclass[12pt, a4paper]{article}\n"
                          "\\usepackage[margin=1in]{geometry}\n"
                          "\\usepackage{mathtext}\n"
                          "\\usepackage[T2A]{fontenc}\n"
                          "\\usepackage[utf8]{inputenc}\n"
                          "\\usepackage{amsmath, amssymb}\n"
                          "\\usepackage[english, russian]{babel}\n"
                          "\n"
                          "\\title{Differentiator By Antonio Tox(a)icity}\n"
                          "\\author{Antonio Tox(a)icity}\n"
                          "\n"
                          "\\begin{document}\n"
                          "\\maketitle\n"
                          "\\setlength{\\parindent}{0pt}\n"
                          "\\large\n"
                          "\n");

    return code_err;
}


static CodeError TexOpen(const char* file_name)
{
    tex_file_ptr = fopen(file_name, "w");
    if (tex_file_ptr == NULL)
    {
        printf(RED "ERROR: can't open file %s" WHT "\n", file_name);
        return FILE_NOT_OPENED_ERR;
    }

    return NO_ERR;
}


#define DEF_OP(__op_name__, __code_for_diff__, __code_for_tex__, ...)           \
    case __op_name__:                                                           \
        {__code_for_tex__}                                                      \
        break;

#define DEF_OP_ONE_ARG(__op_name__, __code_for_diff__, __code_for_tex__, ...)   \
    DEF_OP(__op_name__, __code_for_diff__, __code_for_tex__)

static void TexTree(Node_t* node)
{
    switch(node->type)
    {
        case CONST:
            // printf("NOW I PRINT CONSTANT\n");
            PRINTF_TO_TEX_("%lg", node->value.value_const);
            break;

        case VAR:
            // printf("NOW I PRINT VARIABLE\n");
            PRINTF_TO_TEX_("%c", node->value.value_var);
            break;

        case OP:
            switch (node->value.value_op)
            {
                #include "opers_and_funcs.h"

                default:
                    return;
            }

        default:
            return;
    }
}


#undef PRINTF_TO_TEX_
#undef CHECK_AND_CALL_ERR_
