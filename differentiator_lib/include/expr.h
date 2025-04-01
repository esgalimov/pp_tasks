#ifndef EXPR
#define EXPR

#include "diff.h"
#include "tree.h"
#include "tree_debug.h"
#include "latex.h"
#include "get.h"
#include "read_preorder.h"
#include "print.h"
#include "simplify.h"

const int NAME_MAX_LEN = 32;
const int VARS_MAX_CNT = 32;

//! @brief var_t
typedef struct
{
    char * name;
    elem_t value;
} var_t;

//! @brief Struct to read expression
typedef struct
{
    char*    buffer;
    int      pos;
    int      var_cnt;
    var_t**  vars;
    tree_t*  tree;
} expr_t;

//! @brief Constructor of expr
expr_t* expr_ctor(const char* filename);

//! @brief Func to read file to buffer, return ptr to buffer
char* read_buffer(FILE* stream);

//! @brief Destructor of expr
int expr_dtor(expr_t* expr);

#endif
