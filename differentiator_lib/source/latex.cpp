#include "tree.h"
#include "tree_debug.h"
#include "diff.h"
#include "dsl.h"
#include "latex.h"

FILE* latex_file = NULL;

int open_latex_file(void)
{
    latex_file = fopen("./tmp/main.tex", "w");

    if (graphviz_file == NULL)
    {
        printf("Can't open latex file\n");
        return 1;
    }
    return 0;
}

int close_latex_file(void)
{
    if (latex_file == NULL)
    {
        printf("Latex file has NULL pointer, can't close it\n");
        return 1;
    }
    fprintf(latex_file, "\\end{document}");
    fclose(latex_file);
    return 0;
}

int init_latex_file(void)
{
    ASSERT(latex_file);

    fprintf(latex_file, "\\documentclass{article}\n");
    fprintf(latex_file, "\\usepackage[english]{babel}\n");
    fprintf(latex_file, "\\usepackage[letterpaper,top=2cm,bottom=2cm,left=3cm,right=3cm,marginparwidth=1.75cm]{geometry}\n");
    fprintf(latex_file, "\\DeclareMathSizes{10}{20}{18}{16}\n");
    fprintf(latex_file, "\\usepackage{amsmath}\n");
    fprintf(latex_file, "\\usepackage{graphicx}\n");
    fprintf(latex_file, "\\usepackage[colorlinks=true, allcolors=blue]{hyperref}\n");
    fprintf(latex_file, "\\title{Differentiator by Emil Galimov}\n");
    fprintf(latex_file, "\\begin{document}\n");
    fprintf(latex_file, "\\maketitle\n");
    fprintf(latex_file, "\\section{Function and its derivative}\n");

    return 0;
}

void print_expr_latex(tree_node_t* node, expr_t* expr)
{
    ASSERT(latex_file);

    fprintf(latex_file, "\\begin{center}\n");

    if (expr->var_cnt > 0)
    {
        fprintf(latex_file, "$f( ");

        int i = 0;

        for (; i < expr->var_cnt - 1; i++)
            fprintf(latex_file, "%s, ", expr->vars[i]->name);

        fprintf(latex_file, "%s) = ", expr->vars[i]->name);

        print_subtree_latex(node, expr);
        fprintf(latex_file, "$\\\\\n\\end{center}\n");

        fprintf(latex_file, "\\begin{center}\n");

        for (i = 0; i < expr->var_cnt; i++)
            fprintf(latex_file, "$%s = %lg$\\\\ \n", expr->vars[i]->name, expr->vars[i]->value);

        fprintf(latex_file, "$f( ");

        for (i = 0; i < expr->var_cnt - 1; i++)
            fprintf(latex_file, "%lg, ", expr->vars[i]->value);

        fprintf(latex_file, "%lg) = %lg$", expr->vars[i]->value, eval_var(node, expr));
    }
    else
    {
        fprintf(latex_file, "$f = ");
        print_subtree_latex(node, expr);
        fprintf(latex_file, "= %lg$\\\\", eval(node));
        fprintf(latex_file, "$\\frac{df}{dx} = 0$");
    }

    fprintf(latex_file, "\n\\end{center}\n");

}

void print_diff_latex(tree_node_t* node, expr_t* expr, int var_id)
{
    ASSERT(latex_file);

    fprintf(latex_file, "\\begin{center}\n");

    if (expr->var_cnt == 1)
        fprintf(latex_file, "$\\frac{df}{d%s} = ", expr->vars[var_id]->name);
    else if (expr->var_cnt > 1)
        fprintf(latex_file, "$\\frac{\\partial f}{\\partial %s} = ", expr->vars[var_id]->name);

    print_subtree_latex(node, expr);
    fprintf(latex_file, "$\\\\\n\\end{center}\n");
}

void print_diff_number_latex(tree_node_t* node, expr_t* expr, int var_id)
{
    fprintf(latex_file, "\\begin{center}\n");

    if (expr->var_cnt == 1)
        fprintf(latex_file, "$\\frac{df}{d%s}(", expr->vars[var_id]->name);
    else if (expr->var_cnt > 1)
        fprintf(latex_file, "$\\frac{\\partial f}{\\partial %s}(", expr->vars[var_id]->name);

    int i = 0;
    for (; i < expr->var_cnt - 1; i++)
        fprintf(latex_file, "%lg, ", expr->vars[i]->value);

    fprintf(latex_file, "%lg) = %lg$", expr->vars[i]->value, eval_var(node, expr));

    fprintf(latex_file, "\\\\\n\\end{center}\n");
}

void print_subtree_latex(tree_node_t* node, expr_t* expr)
{
    if (node == NULL) return;

    if (node->type == TYPE_ADD || node->type == TYPE_SUB)
        fprintf(latex_file, "(");
    else if (node->type == TYPE_DIV)
        fprintf(latex_file, "\\frac{");

    print_subtree_latex(node->left, expr);

    if (node->type == TYPE_DIV)
        fprintf(latex_file, "}{");

    switch (node->type)
    {
        case TYPE_NUM: fprintf(latex_file, "%lg", node->value);                        break;
        case TYPE_VAR: fprintf(latex_file, "%s", expr->vars[(int) node->value]->name); break;
        case TYPE_ADD: fprintf(latex_file, "+");                                       break;
        case TYPE_SUB: fprintf(latex_file, "-");                                       break;
        case TYPE_MUL: fprintf(latex_file, " \\cdot ");                                break;
        case TYPE_SIN: fprintf(latex_file, " \\sin{");                                 break;
        case TYPE_COS: fprintf(latex_file, " \\cos{");                                 break;
        case TYPE_LN:  fprintf(latex_file, " \\ln{");                                  break;
        case TYPE_POW: fprintf(latex_file, "^{");                                      break;
        case TYPE_EXP: fprintf(latex_file, " \\exp{");                                 break;
    }

    print_subtree_latex(node->right, expr);
    if ((node->type >= TYPE_SIN && node->type <= TYPE_EXP) || node->type == TYPE_DIV)
        fprintf(latex_file, "}");
    if (node->type == TYPE_ADD || node->type == TYPE_SUB)
        fprintf(latex_file, ")");
}

void create_pdf(void)
{
    close_latex_file();
    system("latex -interaction=batchmode -output-format=pdf ./tmp/main.tex");
    system("open main.pdf");
    system("rm main.log");
    system("rm main.aux");
    system("rm main.out");
}
