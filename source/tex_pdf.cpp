#include "settings.h" // !!!

#include <stdlib.h>

#include "differentiator.h"
#include "formula_scanner.h"
#include "input_output.h"
#include "tex_pdf.h"
#include "tree.h"
#include "utils.h"

#define CHECK_AND_CALL_ERROR_ \
    if (code_err != NO_ERR)   \
        return code_err

#define SPECIAL_PDF_TITLE "Derivatives By Antonio Tox(a)icity"
#define TEX_AUX_FILES_DIR "tex_aux_files/"
#define SPECIAL_TEX_FILE_NAME TEX_AUX_FILES_DIR SPECIAL_PDF_TITLE ".tex"

#define COMMON_PDF_TITLE "Derivatives of functions"
#define COMMON_TEX_FILE_NAME TEX_AUX_FILES_DIR COMMON_PDF_TITLE ".tex"

struct DataForTex
{
    FILE* tex_file_ptr;

    char* formulas_file_buffer;
    int   formulas_buffer_len;
    int   formulas_buffer_idx;

    const char* file_with_formulas_name;
    const char* tex_file_name;

    const char* pdf_title;
    const char* pdf_author_name;
};

static const char* const SPECIAL_FILE_WITH_FORMULAS_NAME = "formulas_for_pdf.txt";
static const char* const SPECIAL_PDF_AUTHOR_NAME         = "Antonio Tox(a)icity";

static const char* const COMMON_FILE_WITH_FORMULAS_NAME  = "example.txt";
static const char* const COMMON_PDF_AUTHOR_NAME          = "Toxic";

static const int         SIMPLE_FORMULAS_NUMBER          = 24;

static CodeError MakeCommonTex             (char var_diff_by);
static CodeError MakeSpecialTex            (char var_diff_by);
static CodeError PrepareTexFile            (DataForTex* tex_data);
static CodeError ScanDiffTexFormulaCommon  (DataForTex* tex_data, char var_diff_by);
static CodeError ScanDiffTexFormulaSpecial (DataForTex* tex_data, char var_diff_by);
static CodeError TexEndAndClose            (DataForTex* tex_data);
static CodeError TexHeader                 (DataForTex* tex_data);
static CodeError TexOpen                   (DataForTex* tex_data);
static void      TexTree                   (FILE*  tex_file_ptr, Node_t* node);


CodeError MakeCommonPdf(char var_diff_by)
{
    CodeError code_err = MakeCommonTex(var_diff_by);
    CHECK_AND_CALL_ERROR_;

    if (system("pdflatex -aux-directory=" TEX_AUX_FILES_DIR " -quiet "
               "\"" COMMON_TEX_FILE_NAME "\"") != 0)
        return TEX_SYSTEM_CALL_ERR;

    return code_err;
}


CodeError MakeSpecialPdf(char var_diff_by)
{
    CodeError code_err = MakeSpecialTex(var_diff_by);
    CHECK_AND_CALL_ERROR_;

    if (system("pdflatex -aux-directory=" TEX_AUX_FILES_DIR " -quiet "
               "\"" SPECIAL_TEX_FILE_NAME "\"") != 0)
        return TEX_SYSTEM_CALL_ERR;

    return code_err;
}


#define PRINTF_TO_TEX_(__format__, ...) \
    fprintf(tex_data.tex_file_ptr, __format__, ##__VA_ARGS__)

static CodeError MakeCommonTex(char var_diff_by)
{
    DataForTex tex_data = {};
    tex_data.file_with_formulas_name = COMMON_FILE_WITH_FORMULAS_NAME;
    tex_data.tex_file_name           = COMMON_TEX_FILE_NAME;
    tex_data.pdf_title               = COMMON_PDF_TITLE;
    tex_data.pdf_author_name         = COMMON_PDF_AUTHOR_NAME;

    CodeError code_err = PrepareTexFile(&tex_data);
    CHECK_AND_CALL_ERROR_;

    while (tex_data.formulas_buffer_idx != tex_data.formulas_buffer_len - 1)
    {
        code_err = ScanDiffTexFormulaCommon(&tex_data, var_diff_by);
        CHECK_AND_CALL_ERROR_;

        if (tex_data.formulas_buffer_idx != tex_data.formulas_buffer_len - 1)
            PRINTF_TO_TEX_("\\ \\\\\\\\\n\n");
    }

    return TexEndAndClose(&tex_data);
}


#define SCAN_DIFF_TEX_FORMULA_SPECIAL_                              \
    code_err = ScanDiffTexFormulaSpecial(&tex_data, var_diff_by);   \
    CHECK_AND_CALL_ERROR_

static CodeError MakeSpecialTex(char var_diff_by)
{
    DataForTex tex_data = {};
    tex_data.file_with_formulas_name = SPECIAL_FILE_WITH_FORMULAS_NAME;
    tex_data.tex_file_name           = SPECIAL_TEX_FILE_NAME;
    tex_data.pdf_title               = SPECIAL_PDF_TITLE;
    tex_data.pdf_author_name         = SPECIAL_PDF_AUTHOR_NAME;

    CodeError code_err = PrepareTexFile(&tex_data);
    CHECK_AND_CALL_ERROR_;

    FILE* tex_file_ptr = fopen(SPECIAL_TEX_FILE_NAME, "w");
    if (tex_file_ptr == NULL)
    {
        printf(RED "ERROR: can't open file \"%s\"" WHT "\n", SPECIAL_TEX_FILE_NAME);
        return FILE_NOT_OPENED_ERR;
    }

    PRINTF_TO_TEX_("Дорогие читатели, это глава пятая (5), часть первая (1) дневника моих похождений по матану.\n"
                   "\n"
                   "Сегодня я хотел бы обсудить с вами очень важную в математическом анализе тему: \\underline{ПРОИЗВОДНАЯ}. "
                   "Давайте начнём с нескольких простых табличных примеров:\n"
                   "\n");

    for (int i = 0; i < SIMPLE_FORMULAS_NUMBER; ++i)
    {
        SCAN_DIFF_TEX_FORMULA_SPECIAL_;
    }

    PRINTF_TO_TEX_("Так, теперь, когда вы разобрались с основой устройства языка Fortran, а также поняли, что такое интеграл, "
                   "предлагаю рассмотреть более интересные случаи применения формулы Ньютона-Лейбница:\n");
    SCAN_DIFF_TEX_FORMULA_SPECIAL_;

    PRINTF_TO_TEX_("Я надеюсь, вам всё понятно? Отлично [растянутое 'o'], тогда продолжаем.\n\n"
                   "Теперь нам необходимо разобраться с теоремой Больцано-Коши. Она формулируется так:\n");
    SCAN_DIFF_TEX_FORMULA_SPECIAL_;

    PRINTF_TO_TEX_("Давайте докажем её? Начнём с очевидного тождества:\n");
    SCAN_DIFF_TEX_FORMULA_SPECIAL_;

    PRINTF_TO_TEX_("Из этого следует такое равенство:\n");
    SCAN_DIFF_TEX_FORMULA_SPECIAL_;

    PRINTF_TO_TEX_("Его можно ещё записать так:\n");
    SCAN_DIFF_TEX_FORMULA_SPECIAL_;

    PRINTF_TO_TEX_("Попробуем упростить ещё? Что ж, отличная идея!\n");
    SCAN_DIFF_TEX_FORMULA_SPECIAL_;

    PRINTF_TO_TEX_("Теперь теорема Лагранжа о среднем доказана! Я вас поздравляю!\n\n"
                   "Однако рано радоваться, поскольку нам требуется раскрыть ещё одну великую тайну!!!\n\n"
                   "Загадка следующая: почему следующее равенство не является опровержением Великой теоремы Ферма?\n");
    SCAN_DIFF_TEX_FORMULA_SPECIAL_;

    return TexEndAndClose(&tex_data);
}

#undef SCAN_DIFF_TEX_FORMULA_SPECIAL_
#undef PRINTF_TO_TEX_


#define PRINTF_TO_TEX_(__format__, ...) \
    fprintf(tex_data->tex_file_ptr, __format__, ##__VA_ARGS__)

static CodeError ScanDiffTexFormulaCommon(DataForTex* tex_data, char var_diff_by)
{
    CodeError code_err = NO_ERR;

    if (tex_data->tex_file_ptr == NULL)
        return PRINT_TO_NULL_PTR_ERR;

    Node_t* func_tree = GetFormulaTree(tex_data->formulas_file_buffer, &tex_data->formulas_buffer_idx);
    CHECK_AND_CALL_ERROR_;

    PRINTF_TO_TEX_("\\begin{equation}\n"
                          "(");
    TexTree(tex_data->tex_file_ptr, func_tree);
    PRINTF_TO_TEX_(")_{%c}^{'} = ", var_diff_by);
    PRINTF_TO_TEX_("\n\\end{equation}\n\n");

    Node_t* diff_func_tree = DiffNode(func_tree, var_diff_by);
    CHECK_AND_CALL_ERROR_;

    DataForSimplification simplify_data = {diff_func_tree,
                                           diff_func_tree,
                                           NO_TEX,
                                           tex_data->tex_file_ptr,
                                           &code_err};
    SimplifyTree(&simplify_data);
    CHECK_AND_CALL_ERROR_;

    PRINTF_TO_TEX_("\\begin{equation*}\n");
    TexTree(tex_data->tex_file_ptr, diff_func_tree);
    PRINTF_TO_TEX_("\n\\end{equation*}\n\n");

    return code_err;
}


static CodeError ScanDiffTexFormulaSpecial(DataForTex* tex_data, char var_diff_by)
{
    CodeError code_err = NO_ERR;

    if (tex_data->tex_file_ptr == NULL)
        return PRINT_TO_NULL_PTR_ERR;

    Node_t* func_tree = GetFormulaTree(tex_data->formulas_file_buffer, &tex_data->formulas_buffer_idx);
    CHECK_AND_CALL_ERROR_;

    PRINTF_TO_TEX_("\\begin{equation}\n"
                          "(");
    TexTree(tex_data->tex_file_ptr, func_tree);
    PRINTF_TO_TEX_(")_{%c}^{'} = ", var_diff_by);

    Node_t* diff_func_tree = DiffNode(func_tree, var_diff_by);

    DataForSimplification simplify_data = {diff_func_tree,
                                           diff_func_tree,
                                           NO_TEX,
                                           NULL,
                                           &code_err};
    SimplifyTree(&simplify_data);
    CHECK_AND_CALL_ERROR_;

    TexTree(tex_data->tex_file_ptr, diff_func_tree);

    PRINTF_TO_TEX_("\n\\end{equation}\n\n");

    return code_err;
}


static CodeError PrepareTexFile(DataForTex* tex_data)
{
    CodeError code_err = NO_ERR;

    tex_data->tex_file_ptr = fopen(SPECIAL_TEX_FILE_NAME, "w");
    if (tex_data->tex_file_ptr == NULL)
    {
        printf(RED "ERROR: can't open file \"%s\"" WHT "\n", SPECIAL_TEX_FILE_NAME);
        return FILE_NOT_OPENED_ERR;
    }

    code_err = MyFread(&tex_data->formulas_file_buffer, &tex_data->formulas_buffer_len,
                        tex_data->file_with_formulas_name);
    CHECK_AND_CALL_ERROR_;

    code_err = TexOpen(tex_data);
    CHECK_AND_CALL_ERROR_;

    code_err = TexHeader(tex_data);
    CHECK_AND_CALL_ERROR_;

    return code_err;
}


static CodeError TexEndAndClose(DataForTex* tex_data)
{
    if (tex_data->tex_file_ptr == NULL)
        return PRINT_TO_NULL_PTR_ERR;

    PRINTF_TO_TEX_("\n\\end{document}\n");

    fclose(tex_data->tex_file_ptr); tex_data->tex_file_ptr = NULL;

    return NO_ERR;
}


static CodeError TexHeader(DataForTex* tex_data)
{
    if (tex_data->tex_file_ptr == NULL)
        return PRINT_TO_NULL_PTR_ERR;

    PRINTF_TO_TEX_("\\documentclass[12pt, a4paper]{article}\n"
                   "\\usepackage[margin=1in]{geometry}\n"
                   "\\usepackage{mathtext}\n"
                   "\\usepackage[T2A]{fontenc}\n"
                   "\\usepackage[utf8]{inputenc}\n"
                   "\\usepackage{amsmath, amssymb}\n"
                   "\\usepackage[english, russian]{babel}\n"
                   "\n"
                   "\\title{%s}\n"
                   "\\author{%s}\n"
                   "\n"
                   "\\begin{document}\n"
                   "\\maketitle\n"
                   "\\setlength{\\parindent}{0pt}\n"
                   "\\large\n"
                   "\n",
                   tex_data->pdf_title, tex_data->pdf_author_name);

    return NO_ERR;
}


static CodeError TexOpen(DataForTex* tex_data)
{
    tex_data->tex_file_ptr = fopen(tex_data->tex_file_name, "w");
    if (tex_data->tex_file_ptr == NULL)
    {
        printf(RED "ERROR: can't open file %s" WHT "\n", tex_data->tex_file_name);
        return FILE_NOT_OPENED_ERR;
    }

    return NO_ERR;
}

#undef PRINTF_TO_TEX_


#define PRINTF_TO_TEX_(__format__, ...) \
    fprintf(tex_file_ptr, __format__, ##__VA_ARGS__)

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


void TexTreeEquation(FILE* tex_file_ptr, Node_t* node)
{
    PRINTF_TO_TEX_("\\begin{equation*}\n");
    TexTree(tex_file_ptr, node);
    PRINTF_TO_TEX_("\n\\end{equation*}\n\n");
}

#undef PRINTF_TO_TEX_

#undef CHECK_AND_CALL_ERROR_
