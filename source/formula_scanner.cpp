#include "settings.h" // !!!

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "formula_scanner.h"
#include "input_output.h"
#include "tree.h"
#include "utils.h"

#define CALL_SYNTAX_ERR \
    { SyntaxError(__FILE__, __PRETTY_FUNCTION__, __LINE__); }

static const int MAX_FUNC_NAME_LEN = 10;

char* file_buffer = NULL;
int   p           = 0;
int   buffer_len  = 0;

static Node_t* GetG();
static Node_t* GetE();
static Node_t* GetT();
static Node_t* GetP();
static Node_t* GetB();
static Node_t* GetS();
static Node_t* GetV();
static Node_t* GetF(const char* func_name);
static Node_t* GetN();

static Node_t* GetFuncNode     (const char* func_name, Node_t* first_arg, Node_t* second_arg);
// static Node_t*   ScanOneNode();
static void    SyntaxError     (const char* file_name, const char* func_name, const int line_number);
static void    UnknownFuncError(const char* file_name, const char* code_func_name,
                                const int line_number, const char* algebr_func_name);

/// -----------------------------------------------------------------------------------------------------------


static Node_t* GetG()
{
    // printf("Enter GetG()\n");
    // printf("%s\n", file_buffer);
    Node_t* node = GetE();

    if (file_buffer[p] != '\r')
        CALL_SYNTAX_ERR;

    ++p;
    if (file_buffer[p] != '\n')
        CALL_SYNTAX_ERR;

    ++p;
    // printf("Leave GetG()\n");
    return node;
}


static Node_t* GetE()
{
    // printf("Enter GetE()\n");

    Node_t* node = GetT();

    if (file_buffer[p] == '+' || file_buffer[p] == '-')
    {
        char op = file_buffer[p];
        ++p;

        Node_t* new_node = GetE();

        if (op == '+')
            return _ADD(node, new_node);
        else
            return _SUB(node, new_node);
    }

    // printf("Leave GetE()\n");
    return node;
}


static Node_t* GetT()
{
    // printf("Enter GetT()\n");

    Node_t* node = GetP();

    if (file_buffer[p] == '*' || file_buffer[p] == '/')
    {
        char op = file_buffer[p];
        ++p;

        Node_t* new_node = GetT();

        if (op == '*')
            return _MUL(node, new_node);
        else
            return _DIV(node, new_node);
    }

    // printf("Leave GetT()\n");
    return node;
}


static Node_t* GetP()
{
    // printf("Enter GetP()\n");

    Node_t* node = GetB();

    if (file_buffer[p] == '^')
    {
        ++p;
        Node_t* new_node = GetP();

        return _POW(node, new_node);
    }

    // printf("Leave GetP()\n");
    return node;
}


static Node_t* GetB()
{
    // printf("Enter GetB()\n");

    if (file_buffer[p] == '(')
    {
        ++p;

        Node_t* node = GetE();
        if (file_buffer[p] != ')')
            CALL_SYNTAX_ERR;

        ++p;
        // printf("Leave GetB() <-- GetE\n");
        return node;
    }

    else
    {
        // printf("Leave GetB() <-- GetS\n");
        return GetS();
    }
}


static Node_t* GetS()
{
    // printf("Enter GetS()\n");

    int old_p = p;
    Node_t* node = GetV();

    if (old_p == p)
        return GetN();

    // printf("Leave GetS()\n");
    return node;
}


static Node_t* GetV()
{
    // printf("Enter GetV()\n");

    int old_p = p;

    char func_or_var_name[MAX_FUNC_NAME_LEN] = {};
    sscanf(&file_buffer[p], "%[a-zA-Z]", func_or_var_name);

    p += (int) strlen(func_or_var_name);

    if (p - old_p == 0)
    {
        // printf("Leave GetV()\n");
        return NULL;
    }

    if (p - old_p == 1 && file_buffer[p] != '(')
    {
        // printf("Leave GetV()\n");
        return _VAR(*func_or_var_name);
    }

    else if (file_buffer[p] == '(')
    {
        ++p;
        Node_t* node = GetF(func_or_var_name);

        if (file_buffer[p] != ')')
            CALL_SYNTAX_ERR;

        ++p;
        // printf("Leave GetV()\n");
        return node;
    }

    CALL_SYNTAX_ERR;
    return NULL;
}


static Node_t* GetF(const char* func_name)
{
    // // printf("Enter GetF()\n");

    Node_t* node = GetE();

    if (file_buffer[p] == ',')
    {
        ++p;
        Node_t* new_node = GetE();
        return GetFuncNode(func_name, node, new_node);
    }

    // printf("Leave GetF()\n");
    return GetFuncNode(func_name, node, NULL);
}


static Node_t* GetN()  // TODO: make double
{
    int val = 0;
    int old_p = p;

    while ('0' <= file_buffer[p] && file_buffer[p] <= '9')
    {
        val = val*10 + file_buffer[p] - '0';
        ++p;
    }

    if (p == old_p)
        CALL_SYNTAX_ERR;

    return _NUM(val);
}


/// -----------------------------------------------------------------------------------------------------------


static Node_t* GetFuncNode(const char* algebr_func_name, Node_t* first_arg, Node_t* second_arg)
{
    if (second_arg != NULL && first_arg != NULL)
    {
        if (strcmp(algebr_func_name, LOG_STR) == 0)
            return _LOG(first_arg, second_arg);
    }

    else if (first_arg != NULL)
    {
        if (strcmp(algebr_func_name, EXP_STR) == 0)
            return _EXP(first_arg);

        else if (strcmp(algebr_func_name, LN_STR) == 0)
            return _LN(first_arg);
    }

    UnknownFuncError(__FILE__, __PRETTY_FUNCTION__, __LINE__, algebr_func_name);
    return NULL;
}


Node_t* ScanFormulaFromFile(const char* file_name, CodeError* p_code_err)
{
    CodeError code_err = NO_ERR;

    if (file_buffer == NULL)
        if ((code_err = MyFread(&file_buffer, &buffer_len, file_name)) != NO_ERR)
        {
            PrintCodeError(code_err);
            return NULL;
        }

    if (p > buffer_len)
    {
        *p_code_err = MET_EOF_DURING_READING_ERR;
        return NULL;
    }

    return GetG();
}


static void SyntaxError(const char* file_name, const char* func_name, const int line_number)
{
    printf(RED "FATAL ERROR:     in file: %s    in func: %s    in line: %d\n"
               "        syntax error: (char = %d, pointer = %d) was met during scanning formula from text file" WHT "\n",
           file_name, func_name, line_number,
           file_buffer[p], p);

    exit(FILE_READING_SYNTAX_ERR);
}


static void UnknownFuncError(const char* file_name, const char* code_func_name,
                             const int line_number, const char* algebr_func_name)
{
    printf(RED "FATAL ERROR:     in file: %s    in func: %s    in line: %d\n"
               "        unknown function (%s) was met during scanning formula from text file" WHT "\n",
           file_name, code_func_name, line_number,
           algebr_func_name);

    exit(MET_UNKNOWN_FUNC_ERR);
}
