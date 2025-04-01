#include "tree.h"
#include "tree_debug.h"
#include "diff.h"
#include "dsl.h"
#include "print.h"

int tree_print_expression(tree_t * tree, mode print_mode, const char * filename)
{
    ASSERT(tree != NULL);

    FILE * fp = fopen(filename, "w");

    if (fp == NULL)
    {
        fprintf(log_file, "<pre>Can't open file \"%s\" for tree print</pre>\n", filename);
        return 1;
    }

    switch (print_mode)
    {
        case PRE:
           tree_print_preorder(tree->root, fp);
           break;
        case IN:
           tree_print_inorder(tree->root, fp);
           break;
        case POST:
        default:
            fprintf(log_file, "<pre>Wrong print mode</pre>\n");
    }

    fclose(fp);
    return 0;
}

void tree_print_preorder(tree_node_t * node, FILE * stream) //(*(+(5)(7))(10))
{
    if (node == NULL) return;

    fprintf(stream, "(");

    if (node->type >= TYPE_ADD && node->type <= TYPE_DIV)
        fprintf(stream, "%c", OPERATIONS[node->type - 1]);
    else if (node->type == TYPE_NUM)
        fprintf(stream, "%lg", node->value);
    else if (node->type == TYPE_VAR)
        fprintf(stream, "%c", (char) node->value);
    else
    {
        switch (node->type)
        {
            case TYPE_SIN: fprintf(stream, "sin("); break;
            case TYPE_COS: fprintf(stream, "cos("); break;
            case TYPE_LN:  fprintf(stream, "ln(");  break;
            case TYPE_POW: fprintf(stream, "^(");   break;
            case TYPE_LOG: fprintf(stream, "log("); break;
            case TYPE_EXP: fprintf(stream, "exp("); break;

        }
    }

    tree_print_preorder(node->left, stream);
    tree_print_preorder(node->right, stream);

    if (node->type >= TYPE_SIN && node->type <= TYPE_EXP)
        fprintf(stream, ")");

    fprintf(stream, ")");
}

void tree_print_inorder(tree_node_t * node, FILE * stream)
{
    if (node == NULL) return;

    if (node->type != TYPE_LOG)
        tree_print_inorder(node->left, stream);

    if (node->type >= TYPE_ADD && node->type <= TYPE_DIV)
        fprintf(stream, "%c", OPERATIONS[node->type - 1]);
    else if (node->type == TYPE_NUM)
        fprintf(stream, "%lg", node->value);
    else if (node->type == TYPE_VAR)
        fprintf(stream, "%c", (char) node->value);
    else
    {
        switch (node->type)
        {
            case TYPE_SIN: fprintf(stream, "sin(");     break;
            case TYPE_COS: fprintf(stream, "cos(");     break;
            case TYPE_LN:  fprintf(stream, "ln(");      break;
            case TYPE_POW: fprintf(stream, "^(");       break;
            case TYPE_LOG: fprintf(stream, "log(");
                tree_print_inorder(node->left, stream); break;
            case TYPE_EXP: fprintf(stream, "exp(");     break;
        }
    }

    tree_print_inorder(node->right, stream);
    if (node->type >= TYPE_SIN && node->type <= TYPE_EXP)
        fprintf(stream, ")");
}
