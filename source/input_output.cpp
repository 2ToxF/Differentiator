#include "settings.h" // !!!

#include <stdio.h>

#include "input_output.h"

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

        ERR_DESCR_(ZERO_DIVISION_ERR);
        ERR_DESCR_(UNKNOWN_NODE_VALUE_TYPE_ERR);

        default:
            printf("\n\n" RED "CODE_ERROR: UNKNOWN_ERROR" WHT "\n");
            break;
    }

    #undef ERR_DESCR_
}
