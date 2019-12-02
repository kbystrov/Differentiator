#ifndef DIFFERENTIATOR_ERR_CODES_H
#define DIFFERENTIATOR_ERR_CODES_H
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/** @file */

#define CHECK_ERROR \
    if(err_code){ \
        return err_code; \
    }

const int ERR_CRT_NODE_IN = 1;
const int ERR_CRT_NODE_TREE = 2;
const int ERR_CRT_NODE_TREE_NUM = 3;
const int ERR_CRT_NODE_MLC = 4;

const int ERR_NODE_OK_NODE = 5;
const int ERR_NODE_OK_TREE = 6;
const int ERR_NODE_OK_PAR_RGHT = 7;
const int ERR_NODE_OK_PAR_LEFT = 8;
const int ERR_NODE_OK_TYPE = 9;
const int ERR_NODE_OK_PAR = 10;
const int ERR_NODE_OK_PAR_TYPE = 11;
const int ERR_NODE_OK_NONE = 12;

const int ERR_TREE_CRT_IN = 12;
const int ERR_TREE_CRT_MAX = 13;
const int ERR_TREE_CRT_MLC = 14;

const int ERR_TREE_DEL_IN = 15;

const int ERR_WRT_TREE_FILE = 16;
const int ERR_WRT_TREE_NODE = 17;
const int ERR_WRT_TREE_FOPEN = 18;

const int ERR_WRT_NODE_FILE = 19;
const int ERR_WRT_NODE_NODE = 20;

const int ERR_TREE_FROM_FILE_TREE = 21;
const int ERR_TREE_FROM_FILE_NAME = 22;
const int ERR_TREE_FROM_FILE_FOPEN = 23;
const int ERR_TREE_FROM_FILE_MLC = 24;
const int ERR_TREE_FROM_FILE_READ_LEN = 25;

const int ERR_GET_VAL_NAME_IN = 26;
const int ERR_GET_VAL_NAME_STR_EMPTY = 27;
const int ERR_GET_VAL_NAME_NOT_FOUND = 28;

const int ERR_IS_VAR_STR_NULL = 29;
const int ERR_IS_VAR_RET_VAL_NULL = 30;
const int ERR_IS_VAR_NOT = 31;

const int ERR_IS_OPER_STR_NULL = 32;
const int ERR_IS_OPER_RET_VAL_NULL = 33;
const int ERR_IS_OPER_NOT = 34;

const int ERR_ADD_NODE_TO_TREE_LEVEL = 35;

const int ERR_GET_NODE_FROM_FILE_RET_POS = 36;
const int ERR_GET_NODE_FROM_FILE_NULL_TERM = 37;
const int ERR_GET_NODE_FILE_BACK_BRACKET = 38;

const int ERR_CPY_STR_BUF = 39;
const int ERR_CPY_STR_RES = 40;
const int ERR_CPY_STR_LEN = 41;

const int ERR_CPY_NODE_DEST = 111;
const int ERR_CPY_NODE_TREE = 112;

const int ERR_CPY_SUB_TREE_DEST = 155;
const int ERR_CPY_SUB_TREE_CHILD = 156;

const int ERR_DIFF_SUB_TREE_DEST = 123;
const int ERR_DIFF_SUB_TREE_OPER = 124;
const int ERR_DIFF_SUB_TREE_TREE = 125;
const int ERR_DIFF_SUB_TREE_POW_ARGS = 126;

const int ERR_FIND_POWER_BOOL = 130;
const int ERR_FIND_POWER_TYPE = 131;

const int ERR_WRT_TEX_FILE = 140;
const int ERR_WRT_TEX_NODE = 141;
const int ERR_WRT_TEX_FOPEN = 142;

const int ERR_WRT_TEX_NODE_FILE = 160;
const int ERR_WRT_TEX_NODE_NODE = 161;
const int ERR_WRT_TEX_NODE_NOT_OP_CHLD =162;
const int ERR_WRT_TEX_NODE_WRONG_TYPE = 163;

#endif //DIFFERENTIATOR_ERR_CODES_H
