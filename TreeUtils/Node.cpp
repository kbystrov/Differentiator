#include "Node.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/** @file */

int CreateNode(Node ** node, elem_t val, DATA_TYPE type, Tree * tree) {
    static size_t id = 0;

    if (node == nullptr){
        return ERR_CRT_NODE_IN;
    }

    if (tree == nullptr){
        return ERR_CRT_NODE_TREE;
    }

    if (tree->nodes_num >= tree->max_nodes){
        return ERR_CRT_NODE_TREE_NUM;
    }

    Node * new_node = (Node *) malloc(sizeof(Node));

    if (new_node == nullptr){
        return ERR_CRT_NODE_MLC;
    }

    new_node->val = val;

    new_node->left = nullptr;
    new_node->right = nullptr;
    new_node->type = type;
    tree->nodes_num++;
    new_node->tree = tree;
    new_node->parent = nullptr;
    new_node->id = id++;

    *node = new_node;

    return 0;
}

int AddChild(Node * parent, elem_t val, DATA_TYPE type, bool_t left) {
    int err_code = 0;
    Node * node = nullptr;

    CheckNode(parent);

    err_code = CreateNode(&node, val, type, parent->tree);
    CHECK_ERROR;

    if(left){
        node->parent = parent;
        parent->left = node;
    } else {
        node->parent = parent;
        parent->right = node;
    }

    err_code = NodeOk(node);
    if(err_code){
        #ifndef NDEBUG
        err_code = NodeTextDump(node);
        #endif
        if(left){
            parent->left = NULL;
        } else {
            parent->right = NULL;
        }
        node->tree->nodes_num--;
        free(node);
        return err_code;
    }

    return err_code;
}

int DeleteNode(Node * node) {
    int err_code = 0;

    CheckNode(node);

    if(node->left){
        DeleteNode(node->left);
    }
    if (node->right){
        DeleteNode(node->right);
    }

    CheckNode(node);

    if(node->parent){
        if(node == node->parent->right){
            node->parent->right = nullptr;
        } else if (node == node->parent->left){
            node->parent->left = nullptr;
        }
    }

    node->tree->nodes_num--;
    free(node);

    return err_code;
}

int CpyNode(Node * node_src, Node ** node_dest, Tree * dest_tree) {
    int err_code = 0;
    Node * new_node = nullptr;

    CheckNode(node_src);

    if(node_dest == nullptr){
        return ERR_CPY_NODE_DEST;
    }

    if(dest_tree == nullptr){
        return ERR_CPY_NODE_TREE;
    }

    err_code = CreateNode(&new_node, node_src->val, node_src->type, dest_tree);
    CHECK_ERROR;

    *node_dest = new_node;

    return err_code;
}
