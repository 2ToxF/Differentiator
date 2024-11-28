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

static void DumpDotHeader(FILE* dot_file);
static void DumpDotFile  (Node_t* node);
static void DumpDotNode  (FILE* dot_file, Node_t* node, Node_t* prev_node, NodesRelation tree_relation);
static void DumpHtmlFile ();
static void SystemCallDot();


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
                case ADD:
                    fprintf(dot_file, "\tp%p[label=\"+\", fillcolor=\"darkmagenta\", fontcolor=\"white\"];\n", node);
                    break;

                case SUB:
                    fprintf(dot_file, "\tp%p[label=\"-\", fillcolor=\"darkmagenta\", fontcolor=\"white\"];\n", node);
                    break;

                case MUL:
                    fprintf(dot_file, "\tp%p[label=\"*\", fillcolor=\"darkmagenta\", fontcolor=\"white\"];\n", node);
                    break;

                case DIV:
                    fprintf(dot_file, "\tp%p[label=\"/\", fillcolor=\"darkmagenta\", fontcolor=\"white\"];\n", node);
                    break;

                case EXP:
                    fprintf(dot_file, "\tp%p[label=\"%s\", fillcolor=\"chartreuse\", fontcolor=\"black\"];\n",
                            node, EXP_STR);
                    break;

                case LN:
                    fprintf(dot_file, "\tp%p[label=\"%s\", fillcolor=\"chartreuse\", fontcolor=\"black\"];\n",
                            node, LN_STR);
                    break;

                case POW:
                    fprintf(dot_file, "\tp%p[label=\"%s\", fillcolor=\"chartreuse\", fontcolor=\"black\"];\n",
                            node, POW_STR);
                    break;

                case LOG:
                    fprintf(dot_file, "\tp%p[label=\"%s\", fillcolor=\"chartreuse\", fontcolor=\"black\"];\n",
                            node, LOG_STR);
                    break;

                case SIN:
                    fprintf(dot_file, "\tp%p[label=\"%s\", fillcolor=\"chartreuse\", fontcolor=\"black\"];\n",
                            node, SIN_STR);
                    break;

                case COS:
                    fprintf(dot_file, "\tp%p[label=\"%s\", fillcolor=\"chartreuse\", fontcolor=\"black\"];\n",
                            node, COS_STR);
                    break;

                case TAN:
                    fprintf(dot_file, "\tp%p[label=\"%s\", fillcolor=\"chartreuse\", fontcolor=\"black\"];\n",
                            node, TAN_STR);
                    break;

                case CTG:
                    fprintf(dot_file, "\tp%p[label=\"%s\", fillcolor=\"chartreuse\", fontcolor=\"black\"];\n",
                            node, CTG_STR);
                    break;

                case SH:
                    fprintf(dot_file, "\tp%p[label=\"%s\", fillcolor=\"chartreuse\", fontcolor=\"black\"];\n",
                            node, SH_STR);
                    break;

                case CH:
                    fprintf(dot_file, "\tp%p[label=\"%s\", fillcolor=\"chartreuse\", fontcolor=\"black\"];\n",
                            node, CH_STR);
                    break;

                case TH:
                    fprintf(dot_file, "\tp%p[label=\"%s\", fillcolor=\"chartreuse\", fontcolor=\"black\"];\n",
                            node, TH_STR);
                    break;

                case CTH:
                    fprintf(dot_file, "\tp%p[label=\"%s\", fillcolor=\"chartreuse\", fontcolor=\"black\"];\n",
                            node, CTH_STR);
                    break;

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
