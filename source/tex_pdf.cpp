#include "settings.h" // !!!

#include <stdlib.h>

#include "differentiator.h"
#include "formula_scanner.h"
#include "input_output.h"
#include "tex_pdf.h"
#include "tree.h"
#include "utils.h"

#define CHECK_AND_CALL_ERR_ \
    if (code_err != NO_ERR) \
        return code_err

#define PRINTF_TO_TEX_(__format__, ...) \
    fprintf(tex_file_ptr, __format__, ##__VA_ARGS__)

#define SCAN_DIFF_TEX_FORMULA_                                                              \
    code_err = ScanDiffTexFormula(tex_file_ptr, formulas_file_buffer, &file_buffer_idx);    \
    CHECK_AND_CALL_ERR_

#define TEX_AUX_FILES_DIR "tex_aux_files/"
#define TEX_FILE_NAME TEX_AUX_FILES_DIR "Differentiator By Antonio Tox(a)icity.tex"

static const char* const INPUT_FILE_WITH_FORMULAS_NAME = "formulas_for_pdf.txt";
static const int         COMMON_FORMULAS_NUMBER        = 24;

static CodeError MakeTexWithFormulas ();
static CodeError ScanDiffTexFormula  (FILE* tex_file_ptr, const char* const formulas_file_buffer,
                                      int* file_buffer_idx);
static CodeError TexEndAndClose      (FILE** p_tex_file_ptr);
static CodeError TexHeader           (FILE*  tex_file_ptr);
static CodeError TexOpen             (FILE** p_tex_file_ptr, const char* file_name);
static void      TexTree             (FILE*  tex_file_ptr, Node_t* node);


CodeError MakePdfWithFormulas()
{
    CodeError code_err = MakeTexWithFormulas();
    CHECK_AND_CALL_ERR_;

    if (system("pdflatex -aux-directory=" TEX_AUX_FILES_DIR " -quiet "
               "\"" TEX_FILE_NAME "\"") != 0)
        return TEX_SYSTEM_CALL_ERR;

    return code_err;
}


static CodeError MakeTexWithFormulas()
{
    CodeError code_err = NO_ERR;

    FILE* tex_file_ptr = fopen(TEX_FILE_NAME, "w");
    if (tex_file_ptr == NULL)
    {
        printf(RED "ERROR: can't open file \"%s\"" WHT "\n", TEX_FILE_NAME);
        return FILE_NOT_OPENED_ERR;
    }

    char* formulas_file_buffer = NULL;
    int   formulas_buffer_len  = 0;
    int   file_buffer_idx      = 0;

    code_err = MyFread(&formulas_file_buffer, &formulas_buffer_len, INPUT_FILE_WITH_FORMULAS_NAME);
    CHECK_AND_CALL_ERR_;

    code_err = TexOpen(&tex_file_ptr, TEX_FILE_NAME);
    CHECK_AND_CALL_ERR_;

    code_err = TexHeader(tex_file_ptr);
    CHECK_AND_CALL_ERR_;

    PRINTF_TO_TEX_("Дорогие читатели, это глава пятая (5), часть первая (1) дневника моих похождений по матану.\n"
                   "\n"
                   "Сегодня я хотел бы обсудить с вами очень важную в математическом анализе тему: \\underline{ПРОИЗВОДНАЯ}. "
                   "Давайте начнём с нескольких простых табличных примеров:\n"
                   "\n");

    for (int i = 0; i < COMMON_FORMULAS_NUMBER; ++i)
    {
        SCAN_DIFF_TEX_FORMULA_;
    }

    PRINTF_TO_TEX_("Так, теперь, когда вы разобрались с основой устройства языка Fortran, а также поняли, что такое интеграл, "
                   "предлагаю рассмотреть более интересные случаи применения формулы Ньютона-Лейбница:\n");
    SCAN_DIFF_TEX_FORMULA_;

    PRINTF_TO_TEX_("Я надеюсь, вам всё понятно? Отлично [растянутое 'o'], тогда продолжаем.\n\n"
                   "Теперь нам необходимо разобраться с теоремой Больцано-Коши. Она формулируется так:\n");
    SCAN_DIFF_TEX_FORMULA_;

    PRINTF_TO_TEX_("Давайте докажем её? Начнём с очевидного тождества:\n");
    SCAN_DIFF_TEX_FORMULA_;

    PRINTF_TO_TEX_("Из этого следует такое равенство:\n");
    SCAN_DIFF_TEX_FORMULA_;

    PRINTF_TO_TEX_("Его можно упростить:\n");
    SCAN_DIFF_TEX_FORMULA_;

    PRINTF_TO_TEX_("Попробуем упростить ещё? Что ж, отличная идея!\n");
    SCAN_DIFF_TEX_FORMULA_;

    PRINTF_TO_TEX_("Теперь теорема Лагранжа о среднем доказана! Я вас поздравляю!\n\n"
                   "Однако рано радоваться, поскольку нам требуется раскрыть ещё одну великую тайну!!!\n\n"
                   "Загадка следующая: почему следующее равенство не является опровержением Великой теоремы Ферма?\n");
    SCAN_DIFF_TEX_FORMULA_;

    return TexEndAndClose(&tex_file_ptr);
}


static CodeError ScanDiffTexFormula(FILE* tex_file_ptr, const char* const formulas_file_buffer,
                                    int* file_buffer_idx)
{
    CodeError code_err = NO_ERR;

    if (tex_file_ptr == NULL)
        return PRINT_TO_NULL_PTR_ERR;

    Node_t* func_tree = GetFormulaTree(formulas_file_buffer, file_buffer_idx);
    code_err = SimplifyTree(func_tree);
    CHECK_AND_CALL_ERR_;

    PRINTF_TO_TEX_("\\begin{equation}\n"
                          "(");
    TexTree(tex_file_ptr, func_tree);
    PRINTF_TO_TEX_(")_{x}^{'} = ");

    Node_t* diff_func_tree = DiffNode(func_tree);
    code_err = SimplifyTree(diff_func_tree);
    CHECK_AND_CALL_ERR_;
    TexTree(tex_file_ptr, diff_func_tree);

    PRINTF_TO_TEX_("\n\\end{equation}\n\n");

    return code_err;
}


static CodeError TexEndAndClose(FILE** p_tex_file_ptr)
{
    FILE* tex_file_ptr = *p_tex_file_ptr;

    if (tex_file_ptr == NULL)
        return PRINT_TO_NULL_PTR_ERR;

    PRINTF_TO_TEX_("\n\\end{document}\n");

    fclose(tex_file_ptr); *p_tex_file_ptr = NULL;

    return NO_ERR;
}


static CodeError TexHeader(FILE* tex_file_ptr)
{
    if (tex_file_ptr == NULL)
        return PRINT_TO_NULL_PTR_ERR;

    PRINTF_TO_TEX_("\\documentclass[12pt, a4paper]{article}\n"
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

    return NO_ERR;
}


static CodeError TexOpen(FILE** tex_file_ptr, const char* file_name)
{
    *tex_file_ptr = fopen(file_name, "w");
    if (*tex_file_ptr == NULL)
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

static void TexTree(FILE* tex_file_ptr, Node_t* node)
{
    switch(node->type)
    {
        case CONST:
            PRINTF_TO_TEX_("%lg", node->value.value_const);
            break;

        case VAR:
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

#undef DEF_OP_ONE_ARG
#undef DEF_OP


#undef PRINTF_TO_TEX_
#undef CHECK_AND_CALL_ERR_
