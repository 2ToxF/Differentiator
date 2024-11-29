#ifndef _UTILS_H
#define _UTILS_H

CodeError Fsize            (const char* file_name, int* input_buffer_length);
bool      IsEqual          (double x1, double x2);
bool      IsZero           (double x);
void      MoveToNextBracket(char** p_file_buffer);

#endif
