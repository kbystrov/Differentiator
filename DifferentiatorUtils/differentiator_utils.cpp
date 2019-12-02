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
                op_div(node_src, new_node, dest_parent, dest_tree, left);
                break;
            case OP_POW:
                //!Additional logic for power operator (when power is constant value and when it is variable - two different derivatives)
                op_pow_simple(node_src, new_node, dest_parent, dest_tree, left);
                if(node_src->left && node_src->right){
                    if(node_src->right->type == VALUE){
                        op_pow_simple(node_src, new_node, dest_parent, dest_tree, left);
                    } else if (node_src->right->type == VARIABLE) {
                        op_pow_complex(node_src, new_node, dest_parent, dest_tree, left);
                    } else {
                        bool is_var = false;
                        err_code = FindPowerType(node_src->right, &is_var);
                        CHECK_ERROR;
                        if (is_var){
                            op_pow_complex(node_src, new_node, dest_parent, dest_tree, left);
                        } else {
                            op_pow_simple(node_src, new_node, dest_parent, dest_tree, left);
                        }
                    }
                } else {
                    return ERR_DIFF_SUB_TREE_POW_ARGS;
                }
                break;
            case OP_EXP:
                op_exp(node_src, new_node, dest_parent, dest_tree, left);
                break;
            case OP_LN:
                op_ln(node_src, new_node, dest_parent, dest_tree, left);
                break;
            case OP_LOG:
                op_log(node_src, new_node, dest_parent, dest_tree, left);
                break;
            case OP_SIN:
                op_sin(node_src, new_node, dest_parent, dest_tree, left);
                break;
            case OP_COS:
                op_cos(node_src, new_node, dest_parent, dest_tree, left);
                break;
            case OP_TAN:
                op_tan(node_src, new_node, dest_parent, dest_tree, left);
                break;
            case OP_CTAN:
                op_ctan(node_src, new_node, dest_parent, dest_tree, left);
                break;
            case OP_ARCSIN:
                op_arcsin(node_src, new_node, dest_parent, dest_tree, left);
                break;
            case OP_ARCCOS:
                op_arccos(node_src, new_node, dest_parent, dest_tree, left);
                break;
            case OP_ARCTAN:
                op_arctan(node_src, new_node, dest_parent, dest_tree, left);
                break;
            case OP_ARCCTAN:
                op_arcctan(node_src, new_node, dest_parent, dest_tree, left);
                break;
            default:
                return ERR_DIFF_SUB_TREE_OPER;
                break;
        };
    } else if (node_src->type == VARIABLE) {
        //!If simple variable than its derivative is 1 and no children are available for variable node
        op_change(dest_parent, 1, VALUE, left, new_node);
    } else {
        //!If simple value constant than its derivative is 0 and no children are available for variable node
        op_change(dest_parent, 0, node_src->type, left, new_node);
    }

    CheckNode(new_node);
    *node_dest = new_node;

    return err_code;
}

int FindPowerType(Node * node, bool * is_var) {
    int err_code = 0;

    CheckNode(node);

    if(is_var == nullptr){
        return ERR_FIND_POWER_BOOL;
    }

    if(node->left){
        switch (node->left->type){
            case VARIABLE:
                *is_var = true;
                return 0;
                break;
            case VALUE:
                break;
            case OPERATOR:
                err_code = FindPowerType(node->left, is_var);
                CHECK_ERROR;
                if(*is_var == true){
                    return 0;
                }
                break;
            default:
                return ERR_FIND_POWER_TYPE;
                break;
        }
    }

    if(node->right){
        switch (node->right->type){
            case VARIABLE:
                *is_var = true;
                return 0;
                break;
            case VALUE:
                break;
            case OPERATOR:
                err_code = FindPowerType(node->right, is_var);
                CHECK_ERROR;
                if(*is_var == true){
                    return 0;
                }
                break;
            default:
                return ERR_FIND_POWER_TYPE;
                break;
        }
    }

    return err_code;
}



