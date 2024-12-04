#include "settings.h" // !!!

#include <stdio.h>
#include <stdlib.h>

#include "input_output.h"
#include "tree_dump.h"
#include "tree.h"

#define DUMP_LOG_PATH "logs/"
#define DUMP_DOT_FILE_NAME DUMP_LOG_PATH "dump_dotfile"
const char* const DUMP_HTML_FILE_NAME = DUMP_LOG_PATH "tree_log.html";

static const int MAX_CMD_LEN       = 310;
static const int MAX_FILE_NAME_LEN = 30;

static char dump_graph_file_name[MAX_FILE_NAME_LEN] = {};

static int   dump_number    = 0;
static FILE* dump_html_file_ptr = NULL;

static void DumpDotHeader (FILE* dot_file);
static void DumpDotFile   (Node_t* node);
static void DumpDotNode   (FILE* dot_file, Node_t* node, Node_t* prev_node, NodesRelation tree_relation);
static void DumpHtmlFile  ();
static void SystemCallDot ();


void DumpClose()
{
    if (dump_html_file_ptr != NULL)
    {
        fclose(dump_html_file_ptr);
        dump_html_file_ptr = NULL;
    }
}


static void DumpDotHeader(FILE* dot_file)
{
    fprintf(dot_file,
            "digraph {\n"
            "\tranksep=\"0.7\"\n"
            "\tnslimit=1;\n"
            "\tnslimit1=1;\n"
            "\trankdir=TB;\n"
            "\tsplines=false;\n"
            "\tbgcolor=\"bisque2\";\n"
            "\tfontname=\"Courier New\";\n"
            "\tnode[shape=\"ellipse\", color=\"coral\", style=\"filled\", fontsize=14];\n"
            "\tedge[color=\"blue\"];\n"
            );
}


static void DumpDotFile(Node_t* node)
{
    FILE* dot_file = fopen(DUMP_DOT_FILE_NAME, "w");

    DumpDotHeader(dot_file);
    DumpDotNode(dot_file, node, NULL, NO_RELATION);
    fprintf(dot_file, "}\n");

    fclose(dot_file); dot_file = NULL;
}


static void DumpDotNode(FILE* dot_file, Node_t* node, Node_t* prev_node, NodesRelation tree_relation)
{
    if (node == NULL)
        return;

    if (node->left != NULL)
        DumpDotNode(dot_file, node->left, node, LEFT_SON);
    if (node->right != NULL)
        DumpDotNode(dot_file, node->right, node, RIGHT_SON);

    switch (node->type)
    {
        case CONST:
            fprintf(dot_file, "\tp%p[label=\"%lg\", fillcolor=\"darkslategray1\", fontcolor=\"black\"];\n",
                    node, node->value.value_const);
            break;

        case VAR:
            fprintf(dot_file, "\tp%p[label=\"%c\", fillcolor=\"brown1\", fontcolor=\"black\"];\n",
                    node, node->value.value_var);
            break;

        case OP:
        {
            switch (node->value.value_op)
            {
                #define DEF_OP(__op_name__, ...)                                                                        \
                    case __op_name__:                                                                                   \
                        fprintf(dot_file, "\tp%p[label=\"%s\", fillcolor=\"darkmagenta\", fontcolor=\"white\"];\n",     \
                                node, __op_name__##_STR);                                                               \
                        break;
                #include "operations.h"
                #undef DEF_OP

                #define DEF_OP(__op_name__, ...)                                                                        \
                    case __op_name__:                                                                                   \
                        fprintf(dot_file, "\tp%p[label=\"%s\", fillcolor=\"chartreuse\", fontcolor=\"black\"];\n",      \
                                node, __op_name__##_STR);                                                               \
                        break;
                #define DEF_OP_ONE_ARG(__op_name__, ...) DEF_OP(__op_name__)
                #include "funcs.h"
                #undef DEF_OP
                #undef DEF_OP_ONE_ARG

                default:
                    break;
            }
            break;
        }

        default:
            break;
    }

    if (prev_node != NULL)
    {
        if (tree_relation == LEFT_SON)
            fprintf(dot_file, "\tp%p:sw -> p%p;\n", prev_node, node);
        else if (tree_relation == RIGHT_SON)
            fprintf(dot_file, "\tp%p:se -> p%p;\n", prev_node, node);
    }
}


static void DumpHtmlFile()
{
    if (dump_html_file_ptr == NULL)
    {
        dump_html_file_ptr = fopen(DUMP_HTML_FILE_NAME, "w");
        if (dump_html_file_ptr == NULL)
            printf(RED "ERROR during dump: FILE WAS NOT OPENED (name of file: %s)" WHT "\n", DUMP_HTML_FILE_NAME);
    }

    fprintf(dump_html_file_ptr,
            "<img src=\"%s\"> <br>\n"
            "-----------------------------------------------------------------------------------------------"
            "-----------------------------------------------------------------------------------------------"
            "------------------------------------------------------------------------------------ <br><br>\n"
            "\n",
            dump_graph_file_name);
}


void TreeDump(Node_t* node)
{
    DumpDotFile(node);
    SystemCallDot();
    DumpHtmlFile();
    ++dump_number;
}


void SystemCallDot()
{
    sprintf(dump_graph_file_name, "tree_graph%d.svg", dump_number);

    char command[MAX_CMD_LEN] = {};
    sprintf(command, "dot -Tsvg " DUMP_DOT_FILE_NAME " -o " DUMP_LOG_PATH "%s", dump_graph_file_name);

    if (system(command) != 0)
    {
        printf(RED "ERROR during dump: SYSTEM CALL ERROR" BLU " (command = %s)" WHT "\n", command);
        return;
    }
}
