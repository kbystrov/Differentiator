#include "Tree.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/** @file */

int CreateTree(Tree ** tree, tree_size_t max_nodes) {

    if(tree == nullptr){
        return  ERR_TREE_CRT_IN;
    }

    if(max_nodes < 1){
        return ERR_TREE_CRT_MAX;
    }

    Tree * new_tree = (Tree *) malloc(sizeof(Tree));

    if(new_tree == nullptr){
        return ERR_TREE_CRT_MLC;
    }

    new_tree->max_nodes = max_nodes;
    new_tree->nodes_num = 0;
    *tree = new_tree;

    return 0;
}

int AddRootFromFile(Tree * tree, char * buf, char ** ret_pos) {
    int err_code = 0;
    char * val = nullptr;
    elem_t ret_val = -1;
    DATA_TYPE type = NONE;
    Node * node = nullptr;

    //! Checking input parameters
    if (tree == nullptr){
        return 666;
    }

    if (tree->root){
        return 666;
    }

    if (buf == nullptr){
        return 666;
    }

    if (ret_pos == nullptr){
        return 666;
    }

    if (*buf != '{'){
        return 666;
    }

    //! Coping root element in string representation
    buf++;
    err_code = CopyStr(&buf, &val);
    if (err_code){
        return err_code;
    }

    //! Determining node type for gotten string
    err_code = DetermNodeType(val, &type, &ret_val);
    if (err_code){
        free(val);
        return err_code;
    }

    //! Creating root element in tree with found node type and value converted to double
    err_code = CreateNode(&node, ret_val, type, tree);
    if(err_code){
        free(val);
        return err_code;
    }
    tree->root = node;
    free(val);
    *ret_pos = buf;

    return err_code;
}

int AddNodeToTree(char * buf, Node * parent, size_t par_level, char ** ret_pos) {
    int err_code = 0;
    size_t i = 0;
    bool_t left = CHILD_NODE - 1;

    //!Checking input parameters
    CheckNode(parent);

    if (buf == nullptr){
        return 666;
    }

    if (ret_pos == nullptr){
        return 666;
    }

    //!Parsing text with brackets and recursively adding children for nodes
    for(i = 0; i < CHILD_NODE; i++){
        Node * node = nullptr;
        //!Parsing char array and getting the position of next node value in it in ret_pos
        err_code = GetNextNodeFromFile(buf, par_level, ret_pos, left);
        if(err_code){
            return err_code;
        }
        buf = *ret_pos;
        //!Creating node, add it as child to input parent node with value read from input buf array and return the position after it in ret_pos
        err_code = CreateNodeFromFile(&node, parent, buf, ret_pos, left);
        if(err_code){
            //!Special case for empty right child and not empty left child
            if (!left && err_code == ERR_CPY_STR_LEN){
                buf = *ret_pos;
                err_code = GetNextNodeFromFile(buf, par_level, ret_pos, left);
                if(err_code != ERR_GET_NODE_FILE_BACK_BRACKET){
                    return err_code;
                } else {
                    buf = *ret_pos;
                    continue;
                }
            } else {
                return err_code;
            }
        }
        buf = *ret_pos;
        //!Recursively call this function for found children
        AddNodeToTree(buf, node, par_level + 1, ret_pos);
        buf = *ret_pos;
        //!Make the same for next child
        left -= 1;
    }

    return err_code;
}

int DeleteTree(Tree * tree) {
    int err_code = 0;

    if(tree == nullptr){
        return ERR_TREE_DEL_IN;
    }

    if(tree->root){
        CheckNode(tree->root);
        err_code = DeleteNode(tree->root);
    }

    free(tree);

    return err_code;
}


int CreateTreeFromFile(Tree ** tree, const char * filename) {
    int err_code = 0;
    FILE * tree_struct = nullptr;
    Tree * new_tree = nullptr;
    size_t input_file_length = 0;
    size_t read_res = 0;
    char * buf= nullptr;
    char * ret_pos = nullptr;

    //!Checking input parameters and creating new tree
    if(tree == nullptr){
        return  ERR_TREE_FROM_FILE_TREE;
    }

    if(filename == nullptr){
        return ERR_TREE_FROM_FILE_NAME;
    }

    if ( !( tree_struct = fopen(filename, "r")) ) {
        return ERR_TREE_FROM_FILE_FOPEN;
    }

    err_code = CreateTree(&new_tree, TREE_NODES);
    CHECK_ERROR;

    //! Get the length of the input file in bytes, allocate the same size buffer and read input file into it
    fseek(tree_struct, 0, SEEK_END);
    input_file_length = ftell(tree_struct);
    rewind(tree_struct);

    buf = (char *) malloc(input_file_length);
    if (buf == nullptr){
        return ERR_TREE_FROM_FILE_MLC;
    }

    read_res = fread(buf, 1, input_file_length, tree_struct);
    if (read_res != input_file_length){
        return ERR_TREE_FROM_FILE_READ_LEN;
    }

    fclose(tree_struct);

    //! Read root node value from input file, add it into tree and recursively searching for children and add them if they were found
    err_code = AddRootFromFile(new_tree, buf, &ret_pos);
    if(err_code){
        free(buf);
        return err_code;
    }
    err_code = AddNodeToTree(ret_pos, new_tree->root, 0, &ret_pos);
    if(err_code){
        *tree = new_tree;
        free(buf);
        return err_code;
    }

    free(buf);

    *tree = new_tree;

    return err_code;
}
