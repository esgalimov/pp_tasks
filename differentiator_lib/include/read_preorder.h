#ifndef READ_PREORDER
#define READ_PREORDER

#include "diff.h"
#include "tree.h"
#include "tree_debug.h"
#include "latex.h"
#include "get.h"
#include "print.h"
#include "simplify.h"
#include "expr.h"

//! @brief Read tree preorder
tree_node_t * tree_read_preorder(char* buffer, int* pos);

//! @brief Check if there is close bracket
int check_close_bracket(char* buffer, int* pos);

//! @brief Get number
tree_node_t* get_number(char* buffer, int* pos);

//! @brief Get +, -, * or /
tree_node_t* get_oper(char* buffer, int* pos, char ch);

//! @brief Get one of funcs or variable
tree_node_t* get_func_or_var(char* buffer, int* pos);

#endif
