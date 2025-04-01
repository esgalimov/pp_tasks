#include "tree.h"
#include "tree_debug.h"
#include "diff.h"
#include "dsl.h"
#include "simplify.h"

void tree_eval_simplify(tree_node_t ** node)
{
    if (*node == NULL) return;
    if (have_var(*node) == 0)
    {
        tree_node_t * old_node = *node;
        *node = NUM(eval(*node));
        (*node)->parent = old_node->parent;
        nodes_dtor(old_node);
        return;
    }
    tree_eval_simplify(&((*node)->left));
    tree_eval_simplify(&((*node)->right));
}

void tree_simplify(tree_t * tree, tree_node_t ** node)
{
    if (*node == NULL) return;

    tree_eval_simplify(node);

    int old_simp = tree->simplify;

    if ((*node)->type == TYPE_MUL)
    {
        if (CMP_LEFT(0) || CMP_RIGHT(0)) num_instead_node(tree, node, 0);
        else if (CMP_LEFT(1))          right_instead_node(tree, node);
        else if (CMP_RIGHT(1))          left_instead_node(tree, node);
    }
    else if ((*node)->type == TYPE_ADD)
    {
        if      (CMP_LEFT(0))  right_instead_node(tree, node);
        else if (CMP_RIGHT(0))  left_instead_node(tree, node);
    }
    else if ((*node)->type == TYPE_SUB)
    {
        if      (CMP_RIGHT(0)) left_instead_node(tree, node);
        else if (CMP_LEFT(0))   neg_instead_node(tree, node);
    }
    else if ((*node)->type == TYPE_DIV)
    {
        if (CMP_RIGHT(0))
        {
            fprintf(log_file, "<pre>ERROR: Division by zero, %p</pre>", node);
            return;
        }
        if      (CMP_LEFT(0))    num_instead_node(tree, node, 0);
        else if (CMP_RIGHT(1))  left_instead_node(tree, node);
    }
    else if ((*node)->type == TYPE_POW)
    {
        if (CMP_LEFT(0) || LEFT_BELOW_ZERO)
        {
            fprintf(log_file, "<pre>ERROR: base of power is below or equal zero, %p</pre>", node);
            return;
        }
        else if (CMP_LEFT(1))   num_instead_node(tree, node, 1);
        else if (CMP_RIGHT(1)) left_instead_node(tree, node);
        else if (CMP_RIGHT(0))  num_instead_node(tree, node, 1);
    }

    tree_simplify(tree, &((*node)->left));
    tree_simplify(tree, &((*node)->right));

    if (tree->simplify > old_simp)
        tree_simplify(tree, node);
}

void right_instead_node(tree_t * tree, tree_node_t ** node)
{
    tree_node_t * old_node = *node;

    *node = old_node->right;
    (*node)->parent = old_node->parent;
    tree->simplify++;

    free(old_node->left);
    free(old_node);
}

void left_instead_node(tree_t * tree, tree_node_t ** node)
{
    tree_node_t * old_node = *node;

    *node = old_node->left;
    (*node)->parent = old_node->parent;
    tree->simplify++;

    free(old_node->right);
    free(old_node);
}

void num_instead_node(tree_t * tree, tree_node_t ** node, elem_t num)
{
    tree_node_t * old_node = *node;

    *node = NUM(num);
    tree->simplify++;

    nodes_dtor(old_node);
}

void neg_instead_node(tree_t * tree, tree_node_t ** node)
{
    tree_node_t * old_node = *node;

    *node = MUL(NUM(-1), old_node->right);
    tree->simplify++;

    free(old_node->left);
    free(old_node);
}

int have_var(tree_node_t * node)
{
    if (node == NULL) return 0;

    if (node->type == TYPE_VAR) return 1;

    int var_cnt = have_var(node->left) + have_var(node->right);

    return var_cnt;
}
