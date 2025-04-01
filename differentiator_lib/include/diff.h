#ifndef DIFF
#define DIFF

#include "tree.h"
#include "tree_debug.h"
#include "latex.h"
#include "get.h"
#include "read_preorder.h"
#include "print.h"
#include "simplify.h"
#include "expr.h"

//! @brief POISON to ctor expr
const int POISON = (int) 0xDEADBEAF;

//! @brief Epsilon to compare double variables
const double EPS = 0.000001;

//! @brief Main func that finds partial derivatives, count them
//! and write to main.tex and create .pdf file
int diff_expression(const char * filename);

//! @brief Check if ch is +, -, * or /
int is_operation(char ch);

//! @brief Eval subtree started with root = node
double eval(const tree_node_t * node);

//! @brief Eval tree with vars values, use value in expr->vars
double eval_var(const tree_node_t* node, expr_t* expr);

//! @brief Func to compare two double numbers, use EPS constant in it
int is_equal(double num1, double num2);

//! @brief Differentiate subtree with root = node by the var with id
//! (it is an index of variable in expr->vars)
tree_node_t * diff(tree_node_t * node, int id);

//! @brief Get var values from asking user in console
int get_var_values(expr_t* expr);

#endif
