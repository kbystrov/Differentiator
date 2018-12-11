#include "read_write_utils.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/** @file */

int GetVarName(elem_t var_ind, const char ** var_name) {
    int i = 0;

    if(var_name == nullptr){
        return ERR_GET_VAL_NAME_IN;
    }

    for (i = 0; i < var_size; i++){
        if (var_ind == variable[i].var){
            const char * name = variable[i].str;
            if (!(*name)){
                return ERR_GET_VAL_NAME_STR_EMPTY;
            } else {
                *var_name = name;
                return 0;
            }
        }
    }

    return ERR_GET_VAL_NAME_NOT_FOUND;
}

int GetOperName(elem_t oper_ind, const char ** oper_name) {
    int i = 0;

    if(oper_name == nullptr){
        return 666;
    }

    for (i = 0; i < oper_size; i++){
        if (oper_ind == oper[i].oper){
            const char * name = oper[i].str;
            if (!(*name)){
                return 666;
            } else {
                *oper_name = name;
                return 0;
            }
        }
    }

    return 666;
}

int IsVar(char * str, elem_t * ret_val) {
    int i = 0;

    if(str == nullptr){
        return ERR_IS_VAR_STR_NULL;
    }

    if(ret_val == nullptr){
        return ERR_IS_VAR_RET_VAL_NULL;
    }

    for (i = 0; i < var_size; i++){
        if (!strcmp(str, variable[i].str)){
            *ret_val = variable[i].var;
            return 0;
        }
    }

    return ERR_IS_VAR_NOT;
}

int IsOperator(char * str, elem_t * ret_val) {
    int i = 0;

    if(str == nullptr){
        return ERR_IS_OPER_STR_NULL;
    }

    if(ret_val == nullptr){
        return ERR_IS_OPER_RET_VAL_NULL;
    }

    for (i = 0; i < oper_size; i++){
        if (!strcmp(str, oper[i].str)){
            *ret_val = oper[i].oper;
            return 0;
        }
    }

    return ERR_IS_OPER_NOT;
}

int CopyStr(char ** buf, char ** res_str) {

    if(buf == nullptr){
        return ERR_CPY_STR_BUF;
    }

    if(res_str == nullptr){
        return ERR_CPY_STR_RES;
    }

    size_t len_str = strcspn(*buf, "\n");
    if (len_str < 1){
        return ERR_CPY_STR_LEN;
    }
    char * val = (char *) calloc(len_str, 1);
    strncpy(val, *buf, len_str);
    *res_str = val;
    *buf = *buf + len_str + 1;

    return 0;
}

int DetermNodeType(char * val, DATA_TYPE * ret_type, elem_t * ret_val) {
    int err_code = 0;
    DATA_TYPE type = NONE;

    while (type == NONE){
        err_code = IsVar(val, ret_val);
        if(err_code){
            if(err_code != ERR_IS_VAR_NOT){
                free(val);
                return err_code;
            }
        } else {
            type = VARIABLE;
            *ret_type = type;
            break;
        }

        err_code = IsOperator(val, ret_val);
        if(err_code){
            if(err_code != ERR_IS_OPER_NOT){
                free(val);
                return err_code;
            }
        } else {
            type = OPERATOR;
            *ret_type = type;
            break;
        }

        *ret_val = strtod(val, NULL);
        type = VALUE;
        *ret_type = type;
        err_code = 0;
    }

    return err_code;
}

int CreateNodeFromFile(Node ** node, Node * parent, char * buf, char ** ret_pos, bool_t left) {
    int err_code = 0;
    elem_t ret_val = -1;
    DATA_TYPE type = NONE;

    if(node == nullptr){
        return 666;
    }

    CheckNode(parent);

    if(buf == nullptr){
        return 666;
    }

    if(ret_pos == nullptr){
        return 666;
    }

    //!Getting new node value to newly allocated string
    char * val = nullptr;
    err_code = CopyStr(&buf, &val);
    if(err_code){
        if (err_code == ERR_CPY_STR_LEN){
            buf++;
            *ret_pos = buf;
        }
        return err_code;
    }


    //! Determining node type for gotten string
    err_code = DetermNodeType(val, &type, &ret_val);
    if (err_code){
        free(val);
        return err_code;
    }

    //! Creating new node, add is as child to input parent into tree and return it as function parameter
    err_code = AddChild(parent, ret_val, type, left);
    if(err_code){
        free(val);
        return err_code;
    }
    if (left){
        *node = parent->left;
    } else {
        *node = parent->right;
    }

    //!Frees allocated string for node value and returns new point in reading buffer
    free(val);
    *ret_pos = buf;

    return err_code;
}

int GetNextNodeFromFile(char * buf, size_t par_level, char ** ret_pos, bool_t left) {
    size_t size = 0;
    size_t level = 0;

    if(ret_pos == nullptr){
        return ERR_GET_NODE_FROM_FILE_RET_POS;
    }

    size = strcspn(buf, "{}");
    buf += size;
    if (!(*buf) ){
        *ret_pos = buf;
        return ERR_GET_NODE_FROM_FILE_NULL_TERM;
    } else if(*buf == '}'){
        if (left){
            *ret_pos = buf + 2;
            return ERR_GET_NODE_FILE_BACK_BRACKET;
        } else {
            while(*buf == '}'){
                *ret_pos = buf + 2;
                buf = *ret_pos;
                size = strcspn(buf, "{}");
                buf += size;
            }
        }
    }

    buf++;

    level = size/BRACE_SPACE_NUM;
    if (level != par_level + 1){
        return ERR_ADD_NODE_TO_TREE_LEVEL;
    }

    *ret_pos = buf;

    return 0;
}
