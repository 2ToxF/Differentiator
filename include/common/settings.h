#ifndef _SETTINGS_H
#define _SETTINGS_H

#define FORMULAS_DIR "formulas/"

const char* const EXP_STR = "exp";
const char* const LN_STR  = "ln";
const char* const POW_STR = "pow";
const char* const LOG_STR = "log";

const char* const SIN_STR = "sin";
const char* const COS_STR = "cos";
const char* const TAN_STR = "tan";
const char* const CTG_STR = "ctg";

const char* const SH_STR  = "sh";
const char* const CH_STR  = "ch";
const char* const TH_STR  = "th";
const char* const CTH_STR = "cth";

const char* const ARCSIN_STR = "arcsin";
const char* const ARCCOS_STR = "arccos";
const char* const ARCTAN_STR = "arctan";
const char* const ARCCTG_STR = "arcctg";

const char* const ARCSH_STR  = "arcsh";
const char* const ARCCH_STR  = "arcch";
const char* const ARCTH_STR  = "arcth";
const char* const ARCCTH_STR = "arccth";

enum CodeError
{
    NO_ERR,

    FILE_NOT_OPENED_ERR,
    FILLING_FILE_STAT_ERR,
    WRONG_INPUT_BUFSIZE_ERR,

    FILE_READING_SYNTAX_ERR,
    MET_EOF_DURING_READING_ERR,
    MET_UNKNOWN_FUNC_ERR,

    ZERO_DIVISION_ERR,
    UNKNOWN_NODE_VALUE_TYPE_ERR,
};

#endif
