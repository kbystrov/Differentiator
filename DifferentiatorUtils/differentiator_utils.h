#ifndef DIFFERENTIATOR_DIFFERENTIATOR_UTILS_H
#define DIFFERENTIATOR_DIFFERENTIATOR_UTILS_H

#include "../Common/data_types.h"
#include "../Common/err_codes.h"
#include "../TreeUtils/Node.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/** @file */

#define differ(src, dest, parent, left) \
    err_code = DifferSubTree(src, dest, parent, left); \
    CHECK_ERROR

#define copy(src, dest, parent, left) \
    err_code = CopySubTree(src, dest, parent, left); \
    CHECK_ERROR

#define op_commut(src, dest, parent) \
    differ(src->left, &dest->left, parent, 1); \
    differ(src->right, &dest->right, parent, 0);

#define op_sub_muls(src, dest_left, dest_right) \
    differ(src->left, &dest_left->left, dest_left, 1); \
    copy(src->right, &dest_left->right, dest_left, 0); \
    copy(src->left, &dest_right->left , dest_right, 1); \
    differ(src->right, &dest_right->right, dest_right, 0);

int CopySubTree(Node * node_src, Node ** node_dest, Node * dest_parent, bool_t left);
int DifferSubTree(Node * node_src, Node ** node_dest, Node * dest_parent, bool_t left);

#endif //DIFFERENTIATOR_DIFFERENTIATOR_UTILS_H
