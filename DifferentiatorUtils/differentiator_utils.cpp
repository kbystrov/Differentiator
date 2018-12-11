#include "differentiator_utils.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/** @file */

int CopySubTree(Node * node_src, Node ** node_dest, Node * dest_parent, bool_t left) {
    int err_code = 0;
    Node * new_node = nullptr;
    Node * left_child = nullptr;
    Node * right_child = nullptr;

    CheckNode(node_src);

    if(node_dest == nullptr){
        return ERR_CPY_SUB_TREE_DEST;
    }

    CheckNode(dest_parent);

    err_code = CpyNode(node_src, &new_node, dest_parent->tree);
    CHECK_ERROR;

    CheckNode(new_node);

    if(left && !dest_parent->left){
        dest_parent->left = new_node;
        new_node->parent = dest_parent;
    } else if (!left && !dest_parent->right){
        dest_parent->right = new_node;
        new_node->parent = dest_parent;
    } else {
        return ERR_CPY_SUB_TREE_CHILD;
    }

    new_node->parent = dest_parent;

    CheckNode(new_node);

    *node_dest = new_node;

    if(node_src->left){
        err_code = CopySubTree(node_src->left, &left_child, new_node, 1);
        CHECK_ERROR;
    }

    if(node_src->right){
        err_code = CopySubTree(node_src->right, &right_child, new_node, 0);
        CHECK_ERROR;
    }

    CheckNode(new_node);

    return err_code;
}

int DifferTreeRoot(Node * node_src, Tree * dest_tree) {
    int err_code = 0;

    //!Checking input parameters
    CheckNode(node_src);

    if(dest_tree == nullptr){
        return ERR_DIFFER_ROOT_TREE;
    }

    if(dest_tree->root || dest_tree->nodes_num){
        return ERR_DIFFER_ROOT_NODES;
    }

    if(node_src->type != OPERATOR){
        return ERR_DIFFER_ROOT_WRONG_TYPE;
    }

    switch( (int) node_src->val){
        case OP_ADD:
        case OP_SUB:

            err_code = CpyNode(node_src, &dest_tree->root, dest_tree);
            CHECK_ERROR;

            if(node_src->left) {
                err_code = DifferSubTree(node_src->left, &dest_tree->root->left, dest_tree->root, dest_tree, 1);
                CHECK_ERROR;
            }

            if(node_src->right) {
                err_code = DifferSubTree(node_src->right, &dest_tree->root->right, dest_tree->root, dest_tree, 0);
                CHECK_ERROR;
            }

            //op_add_sub(node_src, dest_tree->root, NULL, dest_tree, 1);
            break;
        case OP_MUL:
            err_code = CreateNode(&dest_tree->root, OP_ADD, OPERATOR, dest_tree);
            CHECK_ERROR;
            CheckNode(dest_tree->root);
            op_add_op_left(dest_tree->root, OP_MUL);
            op_add_op_right(dest_tree->root, OP_MUL);
            op_sub_muls(node_src, dest_tree->root->left, dest_tree->root->right);
            break;
        case OP_DIV:

        default:
            return ERR_DIFFER_ROOT_WRONG_VALUE;
    }

    return err_code;
}

int DifferSubTree(Node * node_src, Node ** node_dest, Node * dest_parent, Tree * dest_tree, bool_t left) {
    int err_code = 0;
    Node * new_node = nullptr;

    //!Checking input parameters
    CheckNode(node_src);

    if(node_dest == nullptr){
        return ERR_DIFF_SUB_TREE_DEST;
    }

    if(dest_parent){
        CheckNode(dest_parent);
    } else if (dest_tree->root){
        return ERR_DIFF_SUB_TREE_TREE;
    }


    //!Determining type of original node
    if(node_src->type == OPERATOR){
        //!If type is operator than its converting for appropriate form of derivative for such operator (by switch)
        switch((int) node_src->val){
            case OP_ADD:
            case OP_SUB:
                op_add_sub(node_src, new_node, dest_parent, dest_tree, left);
                break;
            case OP_MUL:
                op_mul(node_src, new_node, dest_parent, dest_tree, left);
                break;
            case OP_DIV:
            case OP_POW:
            case OP_EXP:
            case OP_LN:
            default:
                return ERR_DIFF_SUB_TREE_OPER;
                break;
        };
    } else if (node_src->type == VARIABLE) {
        //!If simple variable than its derivative is 1 and no children are available for variable node
        op_change(dest_parent, 1, VALUE, left);
    } else {
        //!If simple value constant than its derivative is 0 and no children are available for variable node
        op_change(dest_parent, 0, node_src->type, left);
    }

    CheckNode(new_node);
    *node_dest = new_node;

    return err_code;
}



