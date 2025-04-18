#include "tree.h"
#include "tree_debug.h"
#include "expr.h"

FILE * graphviz_file = NULL;
FILE * log_file = NULL;
int dump_cnt = 0;

int open_graphviz_file(void)
{
    graphviz_file = fopen("./tmp/graphviz.dot", "w");

    if (graphviz_file == NULL)
    {
        printf("Can't open graphviz file\n");
        return 1;
    }
    return 0;
}

int close_graphviz_file(void)
{
    if (graphviz_file == NULL)
    {
        printf("Graphiz file has NULL pointer, can't close it\n");
        return 1;
    }
    fprintf(graphviz_file, "}");
    fclose(graphviz_file);
    return 0;
}

#ifdef LOG_MODE
    int open_log_file(void)
    {
        log_file = fopen("./logs/log.html", "w");

        if (log_file == NULL)
        {
            printf("Can't open log file\n");
            return 1;
        }

        fprintf(log_file, "<html>\n");
        return 0;
    }

    int close_log_file(void)
    {
        if (log_file == NULL)
        {
            printf("Log file has NULL pointer, can't close it\n");
            return 1;
        }
        fprintf(log_file, "</html>");
        fclose(log_file);
        return 0;
    }
#else
    int open_log_file(void)  { 
        log_file = fopen("/dev/null", "w");

        if (log_file == NULL)
        {
            printf("Can't open log file\n");
            return 1;
        }

        return 0; 
    }
    int close_log_file(void) {
        if (log_file == NULL)
        {
            printf("Log file has NULL pointer, can't close it\n");
            return 1;
        }
        return 0;
    }
#endif

int graphviz_init(tree_t * tree)
{
    ASSERT(tree != NULL);
    ASSERT(graphviz_file != NULL);

    fprintf(graphviz_file, "digraph\n{\n");
    fprintf(graphviz_file, "    node_info[shape = record, label = \"{root = %p}\"];\n\n", tree->root);

    if (tree->root != NULL)
        fprintf(graphviz_file, "    node_info->node_%p [color = \"%s\"];\n", tree->root, GREEN);

    return STATUS_OK;
}

void add_nodes(const tree_node_t * node, expr_t* expr)
{
    if (node == NULL) return;

    if (node->type == TYPE_NUM)
    {
        fprintf(graphviz_file, "    node_%p[shape = Mrecord, label = \"{{%p} | {parent =  %p} | {NUM} | {%lg} | {%p | %p}}\",\n\
                style=\"filled\", fillcolor=\"%s\"];\n", node, node, node->parent, node->value, node->left, node->right, L_GREEN);
    }
    else if (node->type >= TYPE_ADD && node->type <= TYPE_DIV)
    {
        fprintf(graphviz_file, "    node_%p[shape = Mrecord, label = \"{{%p} | {parent =  %p} | {OP} | {%c} | {%p | %p}}\",\n\
                style=\"filled\", fillcolor=\"%s\"];\n", node, node, node->parent, OPERATIONS[node->type - 1], node->left, node->right, L_YELLOW);
    }
    else if (node->type == TYPE_VAR)
    {
        if (expr != NULL)
        {
            fprintf(graphviz_file, "    node_%p[shape = Mrecord, label = \"{{%p} | {parent =  %p} | {VAR} | {%s} | {%p | %p}}\",\n\
                    style=\"filled\", fillcolor=\"%s\"];\n", node, node, node->parent, expr->vars[(int) node->value]->name, node->left, node->right, L_BLUE);
        }
        else
        {
            fprintf(graphviz_file, "    node_%p[shape = Mrecord, label = \"{{%p} | {parent =  %p} | {VAR} | {id = %d} | {%p | %p}}\",\n\
                    style=\"filled\", fillcolor=\"%s\"];\n", node, node, node->parent, (int) node->value, node->left, node->right, L_BLUE);
        }
    }
    else
    {
        fprintf(graphviz_file, "    node_%p[shape = Mrecord, label = \"{{%p} | {parent =  %p} | {FUNC} | {%s} | {%p | %p}}\",\n\
                style=\"filled\", fillcolor=\"%s\"];\n", node, node, node->parent, FUNCS[node->type - 6], node->left, node->right, PINK);
    }
    add_nodes(node->left, expr);
    add_nodes(node->right, expr);
}

void link_nodes_gr(const tree_node_t * node)
{
    if (node == NULL) return;

    if (node->left != NULL)
    {
        fprintf(graphviz_file, "    node_%p->node_%p [color = \"%s\"];\n", node, node->left, GREEN);
        link_nodes_gr(node->left);
    }
    if (node->right != NULL)
    {
        fprintf(graphviz_file, "    node_%p->node_%p [color = \"%s\"];\n", node, node->right, YELLOW);
        link_nodes_gr(node->right);
    }
}

int tree_dump_(tree_t* tree, expr_t* expr, const char* func, const char* file, int line)
{
    ASSERT(tree != NULL);
    ASSERT(log_file != NULL);

    dump_cnt++;
    char graphviz_cmd[200] = "dot ./tmp/graphviz.dot -Tpng -o ./logs/images/tree_dump";
    snprintf(graphviz_cmd + strlen(graphviz_cmd), 30, "%d.png", dump_cnt);

    fprintf(log_file, "<pre>\n%s at %s(%d):\n", func, file, line);

    tree_verify(tree);

    if (tree->status == STATUS_OK)
    {
        fprintf(log_file, "Tree %p (<span style=\"color: green\">OK</span>) \"%s\" at %s at %s(%d):\n{root = %p}</pre>\n",
                tree, tree->info.name, tree->info.func, tree->info.file, tree->info.line, tree->root);
        open_graphviz_file();
        graphviz_init(tree);
        add_nodes(tree->root, expr);
        link_nodes_gr(tree->root);
        close_graphviz_file();
    }
    else
    {
        fprintf(log_file, "Tree %p (<span style=\"color: red\">ERROR</span>) \"%s\" at %s at %s(%d):\n{root = %p}\n",
                tree, tree->info.name, tree->info.func, tree->info.file, tree->info.line, tree->root);
        error_number_translate(tree);
        fprintf(log_file, "</pre>\n");
    }

    system(graphviz_cmd);

    fprintf(log_file, "<img src=\"./images/tree_dump%d.png\">\n", dump_cnt);

    return STATUS_OK;
}

int subtree_dump_(const tree_node_t * node)
{
    ASSERT(log_file != NULL);

    fprintf(log_file, "<pre>Subtree dump: root = %p</pre>\n", node);

    dump_cnt++;
    char graphviz_cmd[200] = "dot ./tmp/graphviz.dot -Tpng -o ./logs/images/tree_dump";
    snprintf(graphviz_cmd + strlen(graphviz_cmd), 30, "%d.png", dump_cnt);

    open_graphviz_file();
    fprintf(graphviz_file, "digraph\n{\n");
    add_nodes(node, NULL);
    link_nodes_gr(node);
    close_graphviz_file();

    system(graphviz_cmd);
    fprintf(log_file, "<img src=\"./images/tree_dump%d.png\">\n", dump_cnt);

    return STATUS_OK;
}

int tree_verify(tree_t * tree)
{
    ASSERT(tree != NULL);

    node_verify(tree, tree->root);

    return STATUS_OK;
}

void node_verify(tree_t * tree, const tree_node_t * node)
{
    if (node == NULL) return;

    if (node->type == TYPE_NUM)
    {
        if (node->left != NULL)
            tree->status |= NUM_NODE_HAS_LEFT_CHILD;

        if (node->right != NULL)
            tree->status |= NUM_NODE_HAS_RIGHT_CHILD;
    }

    if (node == tree->root && tree->root->parent != NULL)
        tree->status |= ROOT_HAVE_PARENT;

    if (node != tree->root && node->parent == NULL)
        tree->status |= NOT_ROOT_HAVE_NO_PARENT;

    if (node->left != NULL && node->left == node->right)
        tree->status |= LEFT_RIGHT_SAME;

    if (node->parent != NULL && node->parent->left != node && node->parent->right != node)
        tree->status |= WRONG_PARENT;

    if (node == node->left || node == node->right)
        tree->status |= CHILD_ITSELF;

    if (node == node->parent)
        tree->status |= PARENT_ITSELF;

    if (tree->status != STATUS_OK)
        return;

    node_verify(tree, node->left);
    node_verify(tree, node->right);
}

void error_number_translate(tree_t * tree)
{
    ASSERT(tree != NULL);

    int i = 0;

    while (i < ERRORS_COUNT)
    {
        switch (tree->status & (1 << i))
        {
            case 0:
                break;
            case NUM_NODE_HAS_LEFT_CHILD:
                fprintf(log_file, "Node with type num has left child\n");
                break;
            case NUM_NODE_HAS_RIGHT_CHILD:
                fprintf(log_file, "Node with type num has right child\n");
                break;
            case ROOT_HAVE_PARENT:
                fprintf(log_file, "Tree root have parent\n");
                break;
            case NOT_ROOT_HAVE_NO_PARENT:
                fprintf(log_file, "Node that is not root has NULL parent\n");
                break;
            case LEFT_RIGHT_SAME:
                fprintf(log_file, "Left and right children are the same\n");
                break;
            case WRONG_PARENT:
                fprintf(log_file, "Parent of node has child of this node\n");
                break;
            case CHILD_ITSELF:
                fprintf(log_file, "Node's child is the same node\n");
                break;
            case PARENT_ITSELF:
                fprintf(log_file, "Node's parent is the same node\n");
                break;
            default:
                fprintf(log_file, "Unknown error\n");
                break;
        }
        i++;
    }
}

// int check_ptr_access(const void * ptr)
// {
//     int fd = open("./tmp/debug.text", O_WRONLY);
//     int ret = (int) write(fd, ptr, 1);
//
//     printf("%d", ret);
//     return ret;
// }
