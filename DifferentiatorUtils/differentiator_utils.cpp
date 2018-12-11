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
    } else if (!left && !dest_parent->right){
        dest_parent->right = new_node;
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

int DifferSubTree(Node * node_src, Node ** node_dest, Node * dest_parent, bool_t left) {
    int err_code = 0;
    Node * new_node = nullptr;
    Node * op_left = nullptr;
    Node * op_right = nullptr;

    //!Checking input parameters
    CheckNode(node_src);

    if(node_dest == nullptr){
        return ERR_DIFF_SUB_TREE_DEST;
    }

    CheckNode(dest_parent);

    //!Determining type of original node
    if(node_src->type == OPERATOR){
        //!If type is operator than its converting for appropriate form of derivative for such operator (by switch)
        switch((int) node_src->val){
            case OP_ADD:
            case OP_SUB:

                err_code = AddChild(dest_parent, node_src->val, node_src->type, left);
                CHECK_ERROR;

                if (left) {
                    new_node = dest_parent->left;
                } else {
                    new_node = dest_parent->right;
                }

                CheckNode(new_node);
                op_commut(node_src, new_node, new_node);

                break;

            case OP_MUL:

                err_code = AddChild(dest_parent, OP_ADD, node_src->type, left);
                CHECK_ERROR;

                if (left) {
                    new_node = dest_parent->left;
                } else {
                    new_node = dest_parent->right;
                }

                CheckNode(new_node);

                err_code = AddChild(new_node, OP_MUL, node_src->type, 1);
                CHECK_ERROR;
                CheckNode(new_node->left);
                op_left = new_node->left;

                err_code = AddChild(new_node, OP_MUL, node_src->type, 0);
                CHECK_ERROR;
                CheckNode(new_node->right);
                op_right = new_node->right;
                /*
                differ(node_src->left, &op_left->left, op_left, 1);
                copy(node_src->right, &op_left->right, op_left, 0);
                copy(node_src->left, &op_right->left , op_right, 1);
                differ(node_src->right, &op_right->right, op_right, 0);
                */

                op_sub_muls(node_src, op_left, op_right);

                break;

            case OP_DIV:
            case OP_POW:
            case OP_EXP:
            case OP_LN:
            default:
                return ERR_DIFF_SUB_TREE_OPER;
                break;
        };
        /*
        err_code = AddChild(dest_parent, node_src->val, node_src->type, left);
        CHECK_ERROR;

        if (left) {
            new_node = dest_parent->left;
        } else {
            new_node = dest_parent->right;
        }

        CheckNode(new_node);

        err_code = DifferSubTree(node_src->left, &left_child_deriv, new_node, 1);
        CHECK_ERROR;
        new_node->left = left_child_deriv;

        err_code = DifferSubTree(node_src->right, &right_child_deriv, new_node, 0);
        CHECK_ERROR;
        new_node->right = right_child_deriv;
        */
    } else if (node_src->type == VARIABLE) {
        //!If simple variable than its derivative is 1 and no children are available for variable node
        err_code = AddChild(dest_parent, 1, VALUE, left);
        CHECK_ERROR;

        if (left) {
            new_node = dest_parent->left;
        } else {
            new_node = dest_parent->right;
        }

    } else {
        //!If simple value constant than its derivative is 0 and no children are available for variable node
        err_code = AddChild(dest_parent, 0, node_src->type, left);
        CHECK_ERROR;

        if (left) {
            new_node = dest_parent->left;
        } else {
            new_node = dest_parent->right;
        }

    }

    CheckNode(new_node);
    *node_dest = new_node;

    return err_code;
}



