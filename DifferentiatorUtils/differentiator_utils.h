#ifndef DIFFERENTIATOR_DIFFERENTIATOR_UTILS_H
#define DIFFERENTIATOR_DIFFERENTIATOR_UTILS_H

#include "../Common/data_types.h"
#include "../Common/err_codes.h"
#include "../TreeUtils/Node.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/** @file */

//!Just simple differentiation of subtree and error check with input params
#define differ(src, dest, parent, left) \
    err_code = DifferSubTree(src, dest, parent, dest_tree, left); \
    CHECK_ERROR
//!Just simple copying of subtree and error check with input params
#define copy(src, dest, parent, left) \
    err_code = CopySubTree(src, dest, parent, left); \
    CHECK_ERROR
//!Case for operators with commutativity of differentiation (Both operands are differentiates separately)
#define op_commut(src, dest, parent) \
    differ(src->left, &dest->left, parent, 1); \
    differ(src->right, &dest->right, parent, 0);
//!Case for operators which requires partial derivatives of left and right
#define op_sub_muls(src, dest_left, dest_right) \
    differ(src->left, &dest_left->left, dest_left, 1); \
    copy(src->right, &dest_left->right, dest_left, 0); \
    copy(src->left, &dest_right->left , dest_right, 1); \
    differ(src->right, &dest_right->right, dest_right, 0);
//!Adds reference of new created node to left or right leaf of parent
#define add_child_ref(node, parent, left) \
    if (left) { \
        node = parent->left; \
    } else { \
        node = parent->right; \
    }
//!Creates node to left or right leaf of parent from input params
#define add_child(parent, val, type, left) \
    err_code = AddChild(parent, val, type, left);
//!Simple copying node and adding it into new tree
#define op_change(parent, val, type, left) \
    add_child(parent, val, type, left); \
    CHECK_ERROR; \
    add_child_ref(new_node, parent, left);
//!Adding new operator to left child
#define op_add_op_left(parent, val) \
    add_child(parent, val, OPERATOR, 1); \
    CHECK_ERROR; \
    CheckNode(parent->left);
//!Adding new operator to right child
#define op_add_op_right(parent, val) \
    add_child(parent, val, OPERATOR, 0); \
    CHECK_ERROR; \
    CheckNode(parent->right);
//!Middle macro for derivative of subroot as combination of two multiplications
#define op_muls(src, dest) \
    op_add_op_left(dest, OP_MUL); \
    op_add_op_right(dest, OP_MUL); \
    op_sub_muls(src, dest->left, dest->right);
//! Differentiation case for ADD and SUB operators
#define op_add_sub(src, dest, parent, tree, left) \
    if (parent){ \
        op_change(parent, src->val, src->type, left); \
    } else { \
        err_code = CpyNode(src, &dest, tree); \
        CHECK_ERROR; \
    }; \
    CheckNode(dest); \
    op_commut(src, dest, dest);
//! Differentiation case for MUL operator
#define op_mul(src, dest, parent, tree, left) \
    if(parent){ \
        op_change(parent, OP_ADD, src->type, left); \
    } else { \
        err_code = CreateNode(&dest, OP_ADD, OPERATOR, tree); \
        CHECK_ERROR; \
    } \
    CheckNode(dest); \
    op_add_op_left(dest, OP_MUL); \
    op_add_op_right(dest, OP_MUL); \
    op_sub_muls(src, dest->left, dest->right);
//! Differentiation case for DIV operator
#define op_div(src, dest, parent, tree, left) \
    if(parent){ \
        op_change(parent, OP_ADD, src->type, left); \
    } else { \
        err_code = CreateNode(&dest, OP_ADD, OPERATOR, tree); \
        CHECK_ERROR; \
    } \
    CheckNode(dest); \
    op_add_op_left(dest, OP_MUL); \
    op_add_op_right(dest, OP_MUL); \
    op_sub_muls(src, dest->left, dest->right);

int CopySubTree(Node * node_src, Node ** node_dest, Node * dest_parent, bool_t left);
int DifferTreeRoot(Node * node_src, Tree * dest_tree);
int DifferSubTree(Node * node_src, Node ** node_dest, Node * dest_parent, Tree * dest_tree, bool_t left);

#endif //DIFFERENTIATOR_DIFFERENTIATOR_UTILS_H
