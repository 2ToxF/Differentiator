#include "settings.h" // !!!

#include <ctype.h>

#include "input_output.h"
#include "tree.h"
#include "utils.h"

#define CALL_SYNTAX_ERR \
    { SyntaxError(__FILE__, __PRETTY_FUNCTION__, __LINE__) }

/// -----------------------------------------------------------------------------------------------------------

// const char* ADD_STR = "add";
// const char* SUB_STR = "sub";
// const char* MUL_STR = "mul";
// const char* DIV_STR = "div";

const char* EXP_STR = "exp";
const char* LN_STR  = "ln";
const char* POW_STR = "pow";
const char* LOG_STR = "log";

const char* SIN_STR = "sin";
const char* COS_STR = "cos";
const char* TAN_STR = "tan";
const char* CTG_STR = "ctg";

const char* SH_STR  = "sh";
const char* CH_STR  = "ch";
const char* TH_STR  = "th";
const char* CTH_STR = "cth";

const char* ARCSIN_STR = "arcsin";
const char* ARCCOS_STR = "arccos";
const char* ARCTAN_STR = "arctan";
const char* ARCCTG_STR = "arcctg";

const char* ARCSH_STR  = "arcsh";
const char* ARCCH_STR  = "arcch";
const char* ARCTH_STR  = "arcth";
const char* ARCCTH_STR = "arccth";

/// -----------------------------------------------------------------------------------------------------------

static const int MAX_FUNC_NAME_LEN = 10;

char* file_buffer  = NULL;
int   file_buf_idx = 0;
int   buffer_len   = 0;

static Node_t*   ScanOneNode();
static CodeError SyntaxError(const char* file_name, const char* func_name, const int line_number);


Node_t* ScanFormulaFromFile(const char* file_name, CodeError* p_code_err)
{
    if (file_buffer == NULL)
        MyFread(&file_buffer, &buffer_len, file_name);

    if (file_buf_idx > buffer_len)
    {
        *p_code_err = MET_EOF_DURING_READING_ERR;
        return NULL;
    }

    Node_t* root = ScanOneNode();
}


#define MOVE_FILE_BUF_PTR_ MoveToNextBracket(&func_op_name)

static Node_t* ScanOneNode()
{
    if (file_buffer[file_buf_idx] == '(')
        ++file_buffer;

    else
    {
        CALL_SYNTAX_ERR;
    }


    double  number        = 0;
    Node_t* left_subtree  = NULL;
    Node_t* right_subtree = NULL;

    if (*file_buffer == '(')
    {
        left_subtree = ScanOneNode();
    }

    else if (sscanf(file_buffer, "%lg", number) != 0)
        return _NUM(number);

    else
    {
        char func_op_name[MAX_FUNC_NAME_LEN] = {};
        sscanf(file_buffer, "%[^()]", func_op_name);

        if (strcmp(func_op_name, "") == 0)
        {
            CALL_SYNTAX_ERR;
        }

        else if (strcmp(func_op_name, EXP_STR) == 0)
        {
            MoveToNextBracket(&func_op_name);
            left_subtree = _EXP(ScanOneNode);
        }

        else if (strcmp(func_op_name, SIN_STR) == 0)
        {
            MoveToNextBracket(&func_op_name);
            left_subtree = _SIN(ScanOneNode);
        }

        else if (strcmp(func_op_name, COS_STR) == 0)
        {
            MoveToNextBracket(&func_op_name);
            left_subtree = _COS(ScanOneNode);
        }
    }

    if (*file_buffer == '+' || *file_buffer == '-' || *file_buffer == '*' || *file_buffer == '/')
    {

    }


    if (*file_buffer == ')')
    {
        ++file_buffer;
        MoveToNextBracket(&file_buffer);
    }

    else
    {
        CALL_SYNTAX_ERR;
    }

    // Node_t* left_node = ScanOneNode();

    // char cur_op = file_buffer[file_buf_idx];
    // switch(cur_op)
    // {
    //     case '+':
    //         Node_t* cur_node = _ADD(left_node, ScanOneNode());

    //     case '-':
    //         Node_t* cur_node = _SUB(left_node, ScanOneNode());

    //     case '*':
    //         Node_t* cur_node = _MUL(left_node, ScanOneNode());

    //     case '/':
    //         Node_t* cur_node = _DIV(left_node, ScanOneNode());

    //     case '^':
    //         Node_t* cur_node = _POW(left_node, ScanOneNode());
    // }
}

#undef MOVE_FILE_BUF_PTR_


static CodeError SyntaxError(const char* file_name, const char* func_name, const int line_number)
{
    printf(RED "FATAL ERROR:     file: %s    func: %s    line: %d    \n"
           MAG "        unknown char (char = %c, pointer = %d) was met during scanning formula from text file",
           file_name, func_name, line_number,
           file_buffer[file_buf_idx], file_buf_idx);

    return FILE_READING_SYNTAX_ERR;
}
