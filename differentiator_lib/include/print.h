#ifndef PRINT
#define PRINT


#include "tree.h"
#include "tree_debug.h"

//! @brief Types of print tree
enum mode
{
    PRE  = 1,
    IN   = 2,
    POST = 3,
};

//! @brief Open file to print and print tree with using next functions
int tree_print_expression(tree_t* tree, mode print_mode, const char * filename);

//! @brief Different type of print tree
//! @param [in] node - ptr to start node
//! @param [out] stream - ptr to file to write
void tree_print_preorder (tree_node_t* node, FILE * stream);
void tree_print_inorder  (tree_node_t* node, FILE * stream);

#endif
