#define ADD(left, right) create_op(TYPE_ADD, left, right)
#define SUB(left, right) create_op(TYPE_SUB, left, right)
#define MUL(left, right) create_op(TYPE_MUL, left, right)
#define DIV(left, right) create_op(TYPE_DIV, left, right)

#define SIN(R)    create_func(TYPE_SIN, R)
#define COS(R)    create_func(TYPE_COS, R)
#define POW(L, R) create_func(TYPE_POW, L, R)
#define LN(R)     create_func(TYPE_LN,  R)
#define LOG(L, R) create_func(TYPE_LOG, L, R)
#define EXP(R)    create_func(TYPE_EXP, R)

#define NUM(x) create_num(x)
#define VAR(x) create_var(x)

#define dL diff(node->left, id)
#define dR diff(node->right, id)

#define cL copy_subtree(node->left)
#define cR copy_subtree(node->right)

#define evalR eval(node->right)
#define evalL eval(node->left)

#define eval_varR eval_var(node->right, expr)
#define eval_varL eval_var(node->left,  expr)

#define CMP_LEFT(x)  ((*node)->left->type  == TYPE_NUM && is_equal((*node)->left->value,  x))

#define CMP_RIGHT(x) ((*node)->right->type == TYPE_NUM && is_equal((*node)->right->value, x))

#define RIGHT_BELOW_ZERO ((*node)->right->type == TYPE_NUM && (*node)->right->value < 0)
#define  LEFT_BELOW_ZERO ((*node)->left->type  == TYPE_NUM && (*node)->left->value  < 0)

