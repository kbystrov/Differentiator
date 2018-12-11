#include "differentiator_utils.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/** @file */

int CopySubTree(Node * node_src, Node ** node_dest, Node * dest_parent, bool_t left) {
    int err_code = 0;
    Node * new_node = nullptr;
    Node * left_child = nullptr;
    Node * right_child = nullptr;

    err_code = NodeOk(node_src);
    if(err_code){
        #ifndef NDEBUG
        err_code = NodeTextDump(node_src);
        #endif
        return err_code;
    }

    if(node_dest == nullptr){
        return ERR_CPY_SUB_TREE_DEST;
    }

    err_code = NodeOk(dest_parent);
    if(err_code){
        #ifndef NDEBUG
        err_code = NodeTextDump(dest_parent);
        #endif
        return err_code;
    }

    err_code = CpyNode(node_src, &new_node, dest_parent->tree);
    if(err_code){
        return err_code;
    }

    err_code = NodeOk(new_node);
    if(err_code){
        #ifndef NDEBUG
        err_code = NodeTextDump(new_node);
        #endif
        return err_code;
    }

    if(left && !dest_parent->left){
        dest_parent->left = new_node;
    } else if (!left && !dest_parent->right){
        dest_parent->right = new_node;
    } else {
        return ERR_CPY_SUB_TREE_CHILD;
    }

    new_node->parent = dest_parent;

    err_code = NodeOk(new_node);
    if(err_code){
        #ifndef NDEBUG
        err_code = NodeTextDump(new_node);
        #endif
        return err_code;
    }

    *node_dest = new_node;

    if(node_src->left){
        err_code = CopySubTree(node_src->left, &left_child, new_node, 1);
        if(err_code){
            return err_code;
        }
    }

    if(node_src->right){
        err_code = CopySubTree(node_src->right, &right_child, new_node, 0);
        if(err_code){
            return err_code;
        }
    }

    return err_code;
}

int DifferSubTree(Node * node_src, Node ** node_dest, Node * dest_parent, Tree * dest_tree, bool_t left) {
    int err_code = 0;
    Node * new_node = nullptr;
    Node * left_child = nullptr;
    Node * right_child = nullptr;

    err_code = NodeOk(node_src);
    if(err_code){
        #ifndef NDEBUG
        err_code = NodeTextDump(node_src);
        #endif
        return err_code;
    }

    if(dest_tree == nullptr){
        return ERR_DIFF_NODE_TREE;
    }

    if(node_dest == nullptr){
        return ERR_DIFF_NODE_DEST;
    }

    err_code = NodeOk(dest_parent);
    if(err_code){
        #ifndef NDEBUG
        err_code = NodeTextDump(dest_parent);
        #endif
        return err_code;
    }

    if(node_src->type == OPERATOR){

        err_code = AddChild(dest_parent, node_src->val, node_src->type, left);
        if(err_code){
            return err_code;
        }

        if (left) {
            new_node = dest_parent->left;
        } else {
            new_node = dest_parent->right;
        }

        err_code = DifferSubTree(node_src->left, &left_child, new_node, dest_tree, 1);
        if (err_code) {
            return err_code;
        }
        new_node->left = left_child;

        err_code = DifferSubTree(node_src->right, &right_child, new_node, dest_tree, 0);
        if (err_code) {
            return err_code;
        }
        new_node->right = right_child;

    } else if (node_src->type == VARIABLE) {

        err_code = AddChild(dest_parent, 1, VALUE, left);
        if(err_code){
            return err_code;
        }

        if (left) {
            new_node = dest_parent->left;
        } else {
            new_node = dest_parent->right;
        }

    } else {

        err_code = AddChild(dest_parent, 0, node_src->type, left);
        if(err_code){
            return err_code;
        }

        if (left) {
            new_node = dest_parent->left;
        } else {
            new_node = dest_parent->right;
        }

    }

    *node_dest = new_node;

    return err_code;
}



