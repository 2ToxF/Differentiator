#include "settings.h" // !!!

#include <stdio.h>

#include "input_output.h"
#include "tree.h"

static const int MAX_STR_VALUE_LEN = 40;

static Node_t* ScanNodeFromFile(FILE* file_ptr);


void PrintCodeError(CodeError code_error)
{
    #define ERR_DESCR_(error)                                   \
        case (error):                                           \
            printf("\n\n" RED "CODE_ERROR: " #error WHT "\n");  \
            break

    switch(code_error)
    {
        case NO_ERR:
            printf("\n\n" GRN "Code was completed without errors" WHT "\n");
            break;

        ERR_DESCR_(FILE_NOT_OPENED_ERR);
        ERR_DESCR_(ZERO_DIVISION_ERR);
        ERR_DESCR_(UNKNOWN_NODE_VALUE_TYPE_ERR);

        default:
            printf("\n\n" RED "CODE_ERROR: UNKNOWN_ERROR" WHT "\n");
            break;
    }

    #undef ERR_DESCR_
}


static Node_t* ScanNodeFromFile(FILE* file_ptr)  // TODO: finish after Huawei on 11.27
{
    char read_info[MAX_STR_VALUE_LEN] = {};
    if (fscanf(file_ptr, "%[^()]", read_info) == 0)
    {
    }
    return NULL;
}


Node_t* ScanTreeFromFile(const char* file_name, CodeError* p_code_err)
{
    FILE* file_ptr = fopen(file_name, "w");
    if (file_ptr == NULL)
    {
        *p_code_err = FILE_NOT_OPENED_ERR;
        return NULL;
    }

    Node_t* root = ScanNodeFromFile(file_ptr);
    return root;
}
