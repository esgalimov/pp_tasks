#ifndef LATEX_FUNCS
#define LATEX_FUNCS


#include "tree.h"
#include "tree_debug.h"
#include "diff.h"
#include "get.h"

extern FILE* latex_file;

int open_latex_file(void);

int close_latex_file(void);

int init_latex_file(void);

void print_expr_latex(tree_node_t* node, expr_t* expr);

void print_diff_latex(tree_node_t* node, expr_t* expr, int var_id);

void print_diff_number_latex(tree_node_t* node, expr_t* expr, int var_id);

void print_subtree_latex(tree_node_t* node, expr_t* expr);

void create_pdf(void);

#endif
