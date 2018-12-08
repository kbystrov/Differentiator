#ifndef DIFFERENTIATOR_NODE_H
#define DIFFERENTIATOR_NODE_H

#include <stdlib.h>
#include "../Common/data_types.h"
#include "../Common/err_codes.h"
#include "../TestUtils/test_utils.h"
#include "Tree.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/** @file */

const size_t CHILD_NODE = 2;

typedef struct NodeStr Node;

//********************************************************************************************************************
#define VAR_DEF(name, num) VAR_##name = num,

//!@def Is used to automatically fill all variables after adding them into "variables.h"
typedef enum{
#include "../Common/variables.h"
} VARS;

#undef VAR_DEF
//********************************************************************************************************************
#define VAR_DEF(name, num) {VAR_##name, #name},

const static struct {
    VARS var;
    const char * str;
} variable [] = {
#include "../Common/variables.h"
};

#undef VAR_DEF

const static int var_size = (sizeof(variable)/sizeof(variable[0]));
//********************************************************************************************************************

//********************************************************************************************************************
#define OP_DEF(name, op, num) OP_##name = num,

//!@def Is used to automatically fill all operators after adding them into "operators.h"
typedef enum{
#include "../Common/operators.h"
} OPS;

#undef OP_DEF
//********************************************************************************************************************
#define OP_DEF(name, op, num) {OP_##name, #op},

const static struct {
    OPS oper;
    const char * str;
} oper [] = {
#include "../Common/operators.h"
};

#undef OP_DEF

const static int oper_size = (sizeof(oper)/sizeof(oper[0]));
//********************************************************************************************************************

struct NodeStr{
    DATA_TYPE type;
    elem_t val;
    Node * left;
    Node * right;
    Node * parent;
    struct TreeStr * tree;
    size_t id;
};

int CreateNode(Node ** node, elem_t val, DATA_TYPE type, struct TreeStr * tree);
int AddChild(Node * parent, elem_t val, DATA_TYPE type, bool_t left);
int DeleteNode(Node * node);

#endif //DIFFERENTIATOR_NODE_H
