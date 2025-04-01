#ifndef SIMPLIFY
#define SIMPLIFY

#include "diff.h"
#include "tree.h"
#include "tree_debug.h"
#include "latex.h"
#include "get.h"
#include "read_preorder.h"
#include "print.h"
#include "expr.h"

//! @brief Func what count subtrees without variables
//! @param [in] node - ptr to start node
//! @return ptr to simplified subtree
void tree_eval_simplify(tree_node_t** node);

//! @brief Func what remove *1, /1, ˆ1, *0, ˆ0, 0ˆ, 1ˆ and count subtrees without variables
//! @param [in] node - ptr to start node
//! @param [in] tree - ptr to tree, it used to change 'simplify'
//! variable to check if tree stop changing and we must stop simplifying
//! @return ptr to simplified subtree
void tree_simplify(tree_t* tree, tree_node_t** node);

//! @brief Func what check if there are variables in subtree
//! @param [in] node - ptr to node
//! @return 1 - there is(are) var(s), 0 - else
int have_var(tree_node_t* node);

//! @brief Replace node with its right child
void right_instead_node(tree_t* tree, tree_node_t** node);

//! @brief Replace node with its left child
void left_instead_node(tree_t* tree, tree_node_t** node);

//! @brief Replace node with given number
void num_instead_node(tree_t* tree, tree_node_t** node, elem_t num);

//! @brief Replace node to its negative value
void neg_instead_node(tree_t* tree, tree_node_t** node);

#endif
