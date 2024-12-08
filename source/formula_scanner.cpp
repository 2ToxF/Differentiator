#include "settings.h" // !!!

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "formula_scanner.h"
#include "input_output.h"
#include "tree.h"

#define CALL_SYNTAX_ERR \
    do { SyntaxError(file_buffer, *p, __FILE__, __PRETTY_FUNCTION__, __LINE__); } while(0)

static const int MAX_FUNC_NAME_LEN = 10;

static Node_t* GetAddSub        (const char* const file_buffer, int* p);
static Node_t* GetMulDiv        (const char* const file_buffer, int* p);
static Node_t* GetPower         (const char* const file_buffer, int* p);
static Node_t* GetBrackets      (const char* const file_buffer, int* p);
static Node_t* GetStringNum     (const char* const file_buffer, int* p);
static Node_t* GetVarFuncNode   (const char* const file_buffer, int* p);
static void    GetVarFuncName   (const char* const file_buffer, int* p, char func_var_name[]);
static Node_t* GetArgsForFunc   (const char* const file_buffer, int* p, const char* const func_name);
static Node_t* GetNumber        (const char* const file_buffer, int* p);

static Node_t* GetFuncNode      (const char* func_name, Node_t* first_arg, Node_t* second_arg);
static void    SyntaxError      (const char* file_buffer, int p, const char* file_name,
                                 const char* func_name, const int line_number);
static void    UnknownFuncError (const char* file_name, const char* code_func_name,
                                const int line_number, const char* algebr_func_name);

/// -----------------------------------------------------------------------------------------------------------


Node_t* GetFormulaTree(const char* const file_buffer, int* p)
{
    Node_t* node = GetAddSub(file_buffer, p);

    if (file_buffer[*p] != '\r')
        CALL_SYNTAX_ERR;

    ++(*p);
    if (file_buffer[*p] != '\n')
        CALL_SYNTAX_ERR;

    ++(*p);
    return node;
}


static Node_t* GetAddSub(const char* const file_buffer, int* p)
{
    Node_t* node = GetMulDiv(file_buffer, p);

    if (file_buffer[*p] == '+' || file_buffer[*p] == '-')
    {
        char op = file_buffer[*p];
        ++(*p);

        Node_t* new_node = GetAddSub(file_buffer, p);

        if (op == '+')
            return _ADD(node, new_node);
        else
            return _SUB(node, new_node);
    }

    return node;
}


static Node_t* GetMulDiv(const char* const file_buffer, int* p)
{
    Node_t* node = GetPower(file_buffer, p);

    if (file_buffer[*p] == '*' || file_buffer[*p] == '/')
    {
        char op = file_buffer[*p];
        ++(*p);

        Node_t* new_node = GetMulDiv(file_buffer, p);

        if (op == '*')
            return _MUL(node, new_node);
        else
            return _DIV(node, new_node);
    }

    return node;
}


static Node_t* GetPower(const char* const file_buffer, int* p)
{
    Node_t* node = GetBrackets(file_buffer, p);

    if (file_buffer[*p] == '^')
    {
        ++(*p);
        Node_t* new_node = GetPower(file_buffer, p);

        return _POW(node, new_node);
    }

    return node;
}


static Node_t* GetBrackets(const char* const file_buffer, int* p)
{
    if (file_buffer[*p] == '(')
    {
        ++(*p);

        Node_t* node = GetAddSub(file_buffer, p);
        if (file_buffer[*p] != ')')
            CALL_SYNTAX_ERR;

        ++(*p);
        return node;
    }

    else
        return GetStringNum(file_buffer, p);
}


static Node_t* GetStringNum(const char* const file_buffer, int* p)
{
    int old_p = *p;
    Node_t* node = GetVarFuncNode(file_buffer, p);

    if (old_p == *p)
        return GetNumber(file_buffer, p);

    return node;
}


static Node_t* GetVarFuncNode(const char* const file_buffer, int* p)  // Вопрос: костыль или не костыль?
{
    int old_p = *p;

    char func_or_var_name[MAX_FUNC_NAME_LEN] = {};
    GetVarFuncName(file_buffer, p, func_or_var_name);

    if (*p - old_p == 0)
        return NULL;

    if (*p - old_p == 1 && file_buffer[*p] != '(')
        return _VAR(*func_or_var_name);

    else if (file_buffer[*p] == '(')
    {
        ++(*p);
        Node_t* node = GetArgsForFunc(file_buffer, p, func_or_var_name);

        if (file_buffer[*p] != ')')
            CALL_SYNTAX_ERR;

        ++(*p);
        return node;
    }

    CALL_SYNTAX_ERR;
    return NULL;
}


static void GetVarFuncName(const char* const file_buffer, int* p, char func_or_var_name[])
{
    int old_p = *p;

    while((('a' <= file_buffer[*p] && file_buffer[*p] <= 'z') ||
           ('A' <= file_buffer[*p] && file_buffer[*p] <= 'Z')) &&
           (*p - old_p < MAX_FUNC_NAME_LEN))
    {
        func_or_var_name[*p - old_p] = file_buffer[*p];
        ++(*p);
    }
}


static Node_t* GetArgsForFunc(const char* const file_buffer, int* p, const char* func_name)
{
    Node_t* node = GetAddSub(file_buffer, p);

    if (file_buffer[*p] == ',')
    {
        ++(*p);
        Node_t* new_node = GetAddSub(file_buffer, p);
        return GetFuncNode(func_name, node, new_node);
    }

    return GetFuncNode(func_name, node, NULL);
}


static Node_t* GetNumber(const char* const file_buffer, int* p)
{
    int val = 0;
    int old_p = *p;

    while ('0' <= file_buffer[*p] && file_buffer[*p] <= '9')
    {
        val = val*10 + file_buffer[*p] - '0';
        ++(*p);
    }

    if (*p == old_p)
        CALL_SYNTAX_ERR;

    return _NUM(val);
}


/// -----------------------------------------------------------------------------------------------------------

static Node_t* GetFuncNode(const char* algebr_func_name, Node_t* first_arg, Node_t* second_arg)
{
    #define DEF_OP(__op_name__, ...)                                        \
        if (strcmp(algebr_func_name, __op_name__##_STR) == 0)               \
            return _##__op_name__(first_arg, second_arg);
    #define DEF_OP_ONE_ARG(__op_name__, ...)

    if (second_arg != NULL && first_arg != NULL)
    {
        #include "funcs.h"
    }

    #undef DEF_OP
    #undef DEF_OP_ONE_ARG


    #define DEF_OP(__op_name__, ...)
    #define DEF_OP_ONE_ARG(__op_name__, ...)                                \
        if (strcmp(algebr_func_name, __op_name__##_STR) == 0)               \
            return _##__op_name__(first_arg);

    else if (first_arg != NULL)
    {
        #include "funcs.h"
    }

    #undef DEF_OP
    #undef DEF_OP_ONE_ARG


    UnknownFuncError(__FILE__, __PRETTY_FUNCTION__, __LINE__, algebr_func_name);
    return NULL;
}


static void SyntaxError(const char* file_buffer, int p, const char* file_name,
                        const char* func_name, const int line_number)
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
