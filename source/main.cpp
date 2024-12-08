#include "settings.h" // !!!

#include "input_output.h"
#include "tex_pdf.h"

int main()
{
    CodeError code_err = MakeCommonPdf();
    // CodeError code_err = MakeSpecialPdf();
    PrintCodeError(code_err);
    return code_err;
}
