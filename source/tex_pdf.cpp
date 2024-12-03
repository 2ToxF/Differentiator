#include "settings.h" // !!!

#include <stdlib.h>

#include "differentiator.h"
#include "formula_scanner.h"
#include "input_output.h"
#include "tex_pdf.h"
#include "tree.h"
#include "utils.h"

#define CHECK_AND_CALL_ERR_                             \
    if (code_err != NO_ERR)                             \
        return code_err

#define PRINTF_TO_TEX_(__format__, ...)                 \
    fprintf(tex_file_ptr, __format__, ##__VA_ARGS__)

#define SCAN_DIFF_TEX_FORMULA_                          \
    code_err = ScanDiffTexFormula();                    \
    CHECK_AND_CALL_ERR_

#define TEX_AUX_FILES_DIR "tex_aux_files/"
#define TEX_FILE_NAME TEX_AUX_FILES_DIR "Differentiator By Antonio Tox(a)icity.tex"

static const char* const INPUT_FILE_WITH_FORMULAS = "formulas.txt";
static const int         COMMON_FORMULAS_NUMBER   = 24;

FILE* tex_file_ptr = NULL;

static CodeError MakeTexWithFormulas();
static CodeError ScanDiffTexFormula ();
static CodeError TexEndAndClose     ();
static CodeError TexHeader          ();
static CodeError TexOpen            (const char* file_name);
static void      TexTree            (Node_t* node);


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

    if (tex_file_ptr == NULL)
    {
        code_err = TexOpen(TEX_FILE_NAME);
        CHECK_AND_CALL_ERR_;

        code_err = TexHeader();
        CHECK_AND_CALL_ERR_;
    }

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

    return TexEndAndClose();
}


static CodeError ScanDiffTexFormula()
{
    CodeError code_err = NO_ERR;

    Node_t* func_tree = ScanFormulaFromFile(INPUT_FILE_WITH_FORMULAS, &code_err);
    CHECK_AND_CALL_ERR_;
    SimplifyTree(func_tree, &code_err);
    CHECK_AND_CALL_ERR_;

    PRINTF_TO_TEX_("\\begin{equation}\n"
                          "(");
    TexTree(func_tree);
    PRINTF_TO_TEX_(")_{x}^{'} = ");

    Node_t* diff_func_tree = DiffNode(func_tree);
    SimplifyTree(diff_func_tree, &code_err);
    CHECK_AND_CALL_ERR_;
    TexTree(diff_func_tree);

    PRINTF_TO_TEX_("\n\\end{equation}\n\n");

    return code_err;
}


static CodeError TexEndAndClose()
{
    if (tex_file_ptr == NULL)
        return TEX_TO_NULL_PTR_ERR;

    PRINTF_TO_TEX_("\n\\end{document}\n");

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


#undef PRINTF_TO_TEX_
#undef CHECK_AND_CALL_ERR_