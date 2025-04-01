#include "tree.h"
#include "tree_debug.h"
#include "diff.h"
#include "dsl.h"
#include "read_preorder.h"

tree_node_t * tree_read_preorder(char * buffer, int * pos)
{
    char ch = '\0';
    int cnt = 0;

    sscanf(buffer + *pos, "%c%n", &ch, &cnt);
    *pos += cnt;

    if (ch == '(')
    {
        sscanf(buffer + *pos, "%c%n", &ch, &cnt);

        if (is_operation(ch))
        {
            *pos += cnt;
            return get_oper(buffer, pos, ch);
        }
        else if (isdigit(ch) || ch == '-')
            return get_number(buffer, pos);

        else if (isalpha(ch) || ch == '^')
            return get_func_or_var(buffer, pos);

        else if (ch == '(')
        {
            *pos += cnt;
            return tree_read_preorder(buffer, pos);
        }

        else
        {
            fprintf(log_file, "<pre>Undefind symbol: %c </pre>\n", ch);
            return NULL;
        }
    }
    else
    {
        fprintf(log_file, "<pre>Must start with \"(\" | ch = %c | pos = %d |</pre>\n", ch, *pos);
        return NULL;
    }
}

int check_close_bracket(char * buffer, int * pos)
{
    int cnt = 0;
    char ch = '\0';

    sscanf(buffer + *pos, "%c%n", &ch, &cnt);

    if (ch != ')')
    {
        fprintf(log_file, "<pre>Must be \")\" after node: pos = %d, ch = %c</pre>\n", *pos, ch);
        return 0;
    }

    return cnt;
}

tree_node_t * get_number(char * buffer, int * pos)
{
    elem_t value = 0;
    int cnt = 0;

    sscanf(buffer + *pos, "%lg%n", &value, &cnt);
    tree_node_t * node_num = create_num(value);
    *pos += cnt;

    cnt = check_close_bracket(buffer, pos);
    if (!cnt)
        return NULL;

    *pos += cnt;
    return node_num;
}

tree_node_t * get_oper(char * buffer, int * pos, char ch)
{
    node_type op = TYPE_MUL;

    switch (ch)
    {
        case '*':
            break;
        case '+':
            op = TYPE_ADD;
            break;
        case '-':
            op = TYPE_SUB;
            break;
        case '/':
            op = TYPE_DIV;
            break;
    }

    tree_node_t * node_ch1 = tree_read_preorder(buffer, pos);
    tree_node_t * node_ch2 = tree_read_preorder(buffer, pos);
    tree_node_t * node_op = create_op(op, node_ch1, node_ch2);

    int cnt = check_close_bracket(buffer, pos);
    if (!cnt)
        return NULL;

    *pos += cnt;

    return node_op;
}

tree_node_t * get_func_or_var(char * buffer, int * pos)
{
    char cmd[32];
    int cnt = 0;
    char ch = '\0';

    sscanf(buffer + *pos, "%[a-z^]%n", cmd, &cnt);
    *pos += cnt;

    sscanf(buffer + *pos, "%c%n", &ch, &cnt);
    int is_var = 0;
    *pos += cnt;

    if (ch != '(')
    {
        is_var = 1;
        *pos -= cnt;
    }

    tree_node_t * node = NULL;

    if (!strcmp(cmd, "sin"))
        node = SIN(tree_read_preorder(buffer, pos));
    else if (!strcmp(cmd, "cos"))
        node = COS(tree_read_preorder(buffer, pos));
    else if (!strcmp(cmd, "ln"))
        node = LN(tree_read_preorder(buffer, pos));
    else if (!strcmp(cmd, "^"))
        node = POW(tree_read_preorder(buffer, pos),
                   tree_read_preorder(buffer, pos));
    else if (!strcmp(cmd, "log"))
        node = LOG(tree_read_preorder(buffer, pos),
                   tree_read_preorder(buffer, pos));
    else if (!strcmp(cmd, "exp"))
        node = EXP(tree_read_preorder(buffer, pos));
    else
    {
        node = VAR(cmd[0]);
        //if (is_var == 0)
    }

    cnt = check_close_bracket(buffer, pos);
    if (!cnt)
        return NULL;
    *pos += cnt;
    if (!is_var)
    {
        cnt = check_close_bracket(buffer, pos);
        if (!cnt)
            return NULL;
        *pos += cnt;
    }

    return node;
}
