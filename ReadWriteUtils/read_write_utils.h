#ifndef DIFFERENTIATOR_READ_WRITE_UTILS_H
#define DIFFERENTIATOR_READ_WRITE_UTILS_H

#include <stdio.h>
#include <string.h>
#include "../Common/data_types.h"
#include "../Common/err_codes.h"
#include "../TreeUtils/Node.h"
#include "../TreeUtils/Tree.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/** @file */

int GetVarName(elem_t var_ind, const char ** var_name);
int GetOperName(elem_t oper_ind, const char ** oper_name);

int IsVar(char * str, elem_t * ret_val);
int IsOperator(char * str, elem_t * ret_val);

int CopyStr(char ** buf, char ** res_str);
int DetermNodeType(char * val, DATA_TYPE * ret_type, elem_t * ret_val);
int CreateNodeFromFile(struct NodeStr ** node, struct NodeStr * parent, char * buf, char ** ret_pos, bool_t left);
int GetNextNodeFromFile(char * buf, size_t par_level, char ** ret_pos, bool_t left);

#endif //DIFFERENTIATOR_READ_WRITE_UTILS_H
