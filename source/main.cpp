#include "settings.h" // !!!

#include "input_output.h"
#include "tex_pdf.h"

char VAR_DIFF_BY = 'x';

int main()
{
    CodeError code_err = MakeCommonPdf(VAR_DIFF_BY);
    // CodeError code_err = MakeSpecialPdf(VAR_DIFF_BY);
    PrintCodeError(code_err);
    return code_err;
}
