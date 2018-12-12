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
#define add_child_ref(node, parent, lefty) \
    if (lefty) { \
        node = parent->left; \
    } else { \
        node = parent->right; \
    }
//!Creates node to left or right leaf of parent from input params
#define add_child(parent, val, type, left) \
    err_code = AddChild(parent, val, type, left);
//!Simple copying node and adding it into new tree
#define op_change(parent, val, type, left, dest) \
    add_child(parent, val, type, left); \
    CHECK_ERROR; \
    add_child_ref(dest, parent, left);
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
        op_change(parent, src->val, src->type, left, dest); \
    } else { \
        err_code = CpyNode(src, &dest, tree); \
        CHECK_ERROR; \
    }; \
    CheckNode(dest); \
    op_commut(src, dest, dest);
//! Differentiation case for MUL operator
#define op_mul(src, dest, parent, tree, left) \
    if(parent){ \
        op_change(parent, OP_ADD, src->type, left, dest); \
    } else { \
        err_code = CreateNode(&dest, OP_ADD, OPERATOR, tree); \
        CHECK_ERROR; \
    } \
    CheckNode(dest); \
    op_muls(src, dest);
//! Differentiation case for DIV operator
#define op_div(src, dest, parent, tree, left) \
    if(parent){ \
        op_change(parent, src->val, src->type, left, dest); \
    } else { \
        err_code = CreateNode(&dest, OP_DIV, OPERATOR, tree); \
        CHECK_ERROR; \
    } \
    CheckNode(dest); \
    op_change(dest, OP_SUB, OPERATOR, 1, dest->left); \
    op_change(dest, OP_MUL, OPERATOR, 0, dest->right); \
    op_muls(src, dest->left); \
    copy(src->right, &dest->right->left, dest->right, 1); \
    copy(src->right, &dest->right->right, dest->right, 0);
//! Differentiation case for LN operator
#define op_ln(src, dest, parent, tree, left) \
    if(parent){ \
        op_change(parent, OP_DIV, src->type, left, dest); \
    } else { \
        err_code = CreateNode(&dest, OP_DIV, OPERATOR, tree); \
        CHECK_ERROR; \
    } \
    CheckNode(dest); \
    differ(src->left, &dest->left, dest, 1); \
    copy(src->left, &dest->right, dest, 0);
//! Differentiation case for EXP operator
#define op_exp(src, dest, parent, tree, left) \
    if(parent){ \
        op_change(parent, OP_MUL, src->type, left, dest); \
    } else { \
        err_code = CreateNode(&dest, OP_MUL, OPERATOR, tree); \
        CHECK_ERROR; \
    } \
    CheckNode(dest); \
    op_change(dest, OP_EXP, OPERATOR, 1, dest->left); \
    copy(src->left, &dest->left->left, dest->left, 1); \
    differ(src->left, &dest->right, dest, 0);
//! Differentiation case for simple POW operator (when power is simple const value)
#define op_pow_simple(src, dest, parent, tree, left) \
    if(parent){ \
        op_change(parent, OP_MUL, src->type, left, dest); \
    } else { \
        err_code = CreateNode(&dest, OP_MUL, OPERATOR, tree); \
        CHECK_ERROR; \
    } \
    CheckNode(dest); \
    copy(src->right, &dest->right, dest, 0); \
    CheckNode(dest->right); \
    op_change(dest, OP_MUL, OPERATOR, 1, dest->left); \
    differ(src->left, &dest->left->right, dest->left, 0); \
    op_change(dest->left, OP_POW, OPERATOR, 1, dest->left->left); \
    copy(src->left, &dest->left->left->left, dest->left->left, 1); \
    op_change(dest->left->left, OP_SUB, OPERATOR, 0, dest->left->left->right); \
    copy(src->right, &dest->left->left->right->left, dest->left->left->right, 1); \
    op_change(dest->left->left->right, 1, VALUE, 0, dest->left->left->right->right);
//! Differentiation case for complex POW operator (when power is variable)
#define op_pow_complex(src, dest, parent, tree, left) \
    if(parent){ \
        op_change(parent, OP_MUL, src->type, left, dest); \
    } else { \
        err_code = CreateNode(&dest, OP_MUL, OPERATOR, tree); \
        CHECK_ERROR; \
    } \
    CheckNode(dest); \
    op_change(dest, OP_LN, OPERATOR, 1, dest->right); \
    CheckNode(dest->right); \
    copy(src->left, &dest->right->left, dest->right, 1); \
    CheckNode(dest->right->left); \
    op_change(dest, OP_MUL, OPERATOR, 1, dest->left); \
    CheckNode(dest->left); \
    differ(src->right, &dest->left->right, dest->left, 0); \
    CheckNode(dest->left->right); \
    op_change(dest->left, OP_POW, OPERATOR, 1, dest->left->left); \
    CheckNode(dest->left->left); \
    copy(src->left, &dest->left->left->left, dest->left->left, 1); \
    copy(src->right, &dest->left->left->right, dest->left->left, 0);
//! Differentiation case for LOG operator
#define op_log(src, dest, parent, tree, left) \
    if(parent){ \
        op_change(parent, OP_DIV, src->type, left, dest); \
    } else { \
        err_code = CreateNode(&dest, OP_DIV, OPERATOR, tree); \
        CHECK_ERROR; \
    } \
    CheckNode(dest); \
    differ(src->right, &dest->left, dest, 1); \
    CheckNode(dest->left); \
    op_change(dest, OP_MUL, OPERATOR, 0, dest->right); \
    CheckNode(dest->left); \
    copy(src->right, &dest->right->left, dest->right, 1); \
    CheckNode(dest->right->left); \
    op_change(dest->right, OP_LN, OPERATOR, 0, dest->right->right); \
    CheckNode(dest->right->right); \
    copy(src->left, &dest->right->right->left, dest->right->right, 1); \
    CheckNode(dest->right->right->left);
//! Differentiation case for SIN operator
#define op_sin(src, dest, parent, tree, left) \
    if(parent){ \
        op_change(parent, OP_MUL, src->type, left, dest); \
    } else { \
        err_code = CreateNode(&dest, OP_MUL, OPERATOR, tree); \
        CHECK_ERROR; \
    } \
    CheckNode(dest); \
    op_change(dest, OP_COS, OPERATOR, 1, dest->left); \
    CheckNode(dest->left); \
    copy(src->left, &dest->left->left, dest->left, 1); \
    CheckNode(dest->left->left); \
    differ(src->left, &dest->right, dest, 0); \
    CheckNode(dest->right);
//! Differentiation case for COS operator
#define op_cos(src, dest, parent, tree, left) \
    if(parent){ \
        op_change(parent, OP_MUL, src->type, left, dest); \
    } else { \
        err_code = CreateNode(&dest, OP_MUL, OPERATOR, tree); \
        CHECK_ERROR; \
    } \
    CheckNode(dest); \
    op_change(dest, OP_SUB, OPERATOR, 1, dest->left); \
    CheckNode(dest->left); \
    op_change(dest->left, 0, VALUE, 1, dest->left->left); \
    CheckNode(dest->left->left); \
    op_change(dest->left, OP_SIN, OPERATOR, 0, dest->left->right); \
    CheckNode(dest->left->right); \
    copy(src->left, &dest->left->right->left, dest->left->right, 1); \
    CheckNode(dest->left->right->left); \
    differ(src->left, &dest->right, dest, 0); \
    CheckNode(dest->right);
//! Differentiation case for TAN operator
#define op_tan(src, dest, parent, tree, left) \
    if(parent){ \
        op_change(parent, OP_DIV, src->type, left, dest); \
    } else { \
        err_code = CreateNode(&dest, OP_DIV, OPERATOR, tree); \
        CHECK_ERROR; \
    } \
    CheckNode(dest); \
    differ(src->left, &dest->left, dest, 1); \
    CheckNode(dest->left); \
    op_change(dest, OP_MUL, OPERATOR, 0, dest->right); \
    CheckNode(dest->right); \
    op_change(dest->right, OP_COS, OPERATOR, 1, dest->right->left); \
    CheckNode(dest->right->left); \
    op_change(dest->right, OP_COS, OPERATOR, 0, dest->right->right); \
    CheckNode(dest->right->right); \
    copy(src->left, &dest->right->left->left, dest->right->left, 1); \
    CheckNode(dest->right->left->left); \
    copy(src->left, &dest->right->right->left, dest->right->right, 1); \
    CheckNode(dest->right->right->left);
//! Differentiation case for CTAN operator
#define op_ctan(src, dest, parent, tree, left) \
    if(parent){ \
        op_change(parent, OP_DIV, src->type, left, dest); \
    } else { \
        err_code = CreateNode(&dest, OP_DIV, OPERATOR, tree); \
        CHECK_ERROR; \
    } \
    CheckNode(dest); \
    op_change(dest, OP_SUB, OPERATOR, 1, dest->left); \
    CheckNode(dest->left); \
    op_change(dest->left, 0, VALUE, 1, dest->left->left); \
    CheckNode(dest->left->left); \
    differ(src->left, &dest->left->right, dest->left, 0); \
    CheckNode(dest->left->right); \
    op_change(dest, OP_MUL, OPERATOR, 0, dest->right); \
    CheckNode(dest->right); \
    op_change(dest->right, OP_SIN, OPERATOR, 1, dest->right->left); \
    CheckNode(dest->right->left); \
    op_change(dest->right, OP_SIN, OPERATOR, 0, dest->right->right); \
    CheckNode(dest->right->right); \
    copy(src->left, &dest->right->left->left, dest->right->left, 1); \
    CheckNode(dest->right->left->left); \
    copy(src->left, &dest->right->right->left, dest->right->right, 1); \
    CheckNode(dest->right->right->left);
//! Differentiation case for ARCSIN operator
#define op_arcsin(src, dest, parent, tree, left) \
    if(parent){ \
        op_change(parent, OP_DIV, src->type, left, dest); \
    } else { \
        err_code = CreateNode(&dest, OP_DIV, OPERATOR, tree); \
        CHECK_ERROR; \
    } \
    CheckNode(dest); \
    differ(src->left, &dest->left, dest, 1); \
    CheckNode(dest->left); \
    op_change(dest, OP_POW, OPERATOR, 0, dest->right); \
    CheckNode(dest->right); \
    op_change(dest->right, OP_SUB, OPERATOR, 1, dest->right->left); \
    CheckNode(dest->right->left); \
    op_change(dest->right->left, 1, VALUE, 1, dest->right->left->left); \
    CheckNode(dest->right->left->left); \
    op_change(dest->right->left, OP_MUL, OPERATOR, 0, dest->right->left->right); \
    CheckNode(dest->right->left->right); \
    copy(src->left, &dest->right->left->right->left, dest->right->left->right, 1); \
    CheckNode(dest->right->left->right->left); \
    copy(src->left, &dest->right->left->right->right, dest->right->left->right, 0); \
    CheckNode(dest->right->left->right->right); \
    op_change(dest->right, 0.5, VALUE, 0, dest->right->right); \
    CheckNode(dest->right->right);
//! Differentiation case for ARCCOS operator
#define op_arccos(src, dest, parent, tree, left) \
    if(parent){ \
        op_change(parent, OP_DIV, src->type, left, dest); \
    } else { \
        err_code = CreateNode(&dest, OP_DIV, OPERATOR, tree); \
        CHECK_ERROR; \
    } \
    CheckNode(dest); \
    op_change(dest, OP_SUB, OPERATOR, 1, dest->left); \
    CheckNode(dest->left); \
    op_change(dest->left, 0, VALUE, 1, dest->left->left); \
    CheckNode(dest->left->left); \
    differ(src->left, &dest->left->right, dest->left, 0); \
    CheckNode(dest->left->right); \
    op_change(dest, OP_POW, OPERATOR, 0, dest->right); \
    CheckNode(dest->right); \
    op_change(dest->right, OP_SUB, OPERATOR, 1, dest->right->left); \
    CheckNode(dest->right->left); \
    op_change(dest->right->left, 1, VALUE, 1, dest->right->left->left); \
    CheckNode(dest->right->left->left); \
    op_change(dest->right->left, OP_MUL, OPERATOR, 0, dest->right->left->right); \
    CheckNode(dest->right->left->right); \
    copy(src->left, &dest->right->left->right->left, dest->right->left->right, 1); \
    CheckNode(dest->right->left->right->left); \
    copy(src->left, &dest->right->left->right->right, dest->right->left->right, 0); \
    CheckNode(dest->right->left->right->right); \
    op_change(dest->right, 0.5, VALUE, 0, dest->right->right); \
    CheckNode(dest->right->right);
//! Differentiation case for ARCTAN operator
#define op_arctan(src, dest, parent, tree, left) \
    if(parent){ \
        op_change(parent, OP_DIV, src->type, left, dest); \
    } else { \
        err_code = CreateNode(&dest, OP_DIV, OPERATOR, tree); \
        CHECK_ERROR; \
    } \
    CheckNode(dest); \
    differ(src->left, &dest->left, dest, 1); \
    CheckNode(dest->left); \
    op_change(dest, OP_ADD, OPERATOR, 0, dest->right); \
    CheckNode(dest->right); \
    op_change(dest->right, 1, VALUE, 1, dest->right->left); \
    CheckNode(dest->right->left); \
    op_change(dest->right, OP_MUL, OPERATOR, 0, dest->right->right); \
    CheckNode(dest->right->right); \
    copy(src->left, &dest->right->right->left, dest->right->right, 1); \
    CheckNode(dest->right->right->left); \
    copy(src->left, &dest->right->right->right, dest->right->right, 0); \
    CheckNode(dest->right->right->right);
//! Differentiation case for ARCCTAN operator
#define op_arcctan(src, dest, parent, tree, left) \
    if(parent){ \
        op_change(parent, OP_DIV, src->type, left, dest); \
    } else { \
        err_code = CreateNode(&dest, OP_DIV, OPERATOR, tree); \
        CHECK_ERROR; \
    } \
    CheckNode(dest); \
    op_change(dest, OP_SUB, OPERATOR, 1, dest->left); \
    CheckNode(dest->left); \
    op_change(dest->left, 0, VALUE, 1, dest->left->left); \
    CheckNode(dest->left->left); \
    differ(src->left, &dest->left->right, dest->left, 0); \
    CheckNode(dest->left->right); \
    op_change(dest, OP_ADD, OPERATOR, 0, dest->right); \
    CheckNode(dest->right); \
    op_change(dest->right, 1, VALUE, 1, dest->right->left); \
    CheckNode(dest->right->left); \
    op_change(dest->right, OP_MUL, OPERATOR, 0, dest->right->right); \
    CheckNode(dest->right->right); \
    copy(src->left, &dest->right->right->left, dest->right->right, 1); \
    CheckNode(dest->right->right->left); \
    copy(src->left, &dest->right->right->right, dest->right->right, 0); \
    CheckNode(dest->right->right->right);

int CopySubTree(Node * node_src, Node ** node_dest, Node * dest_parent, bool_t left);
int DifferSubTree(Node * node_src, Node ** node_dest, Node * dest_parent, Tree * dest_tree, bool_t left);
int FindPowerType(Node * node, bool * is_var);

#endif //DIFFERENTIATOR_DIFFERENTIATOR_UTILS_H
