#ifndef DIFFERENTIATOR_DIFFERENTIATOR_UTILS_H
#define DIFFERENTIATOR_DIFFERENTIATOR_UTILS_H

#include "../Common/data_types.h"
#include "../Common/err_codes.h"
#include "../TreeUtils/Node.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/** @file */

int CopySubTree(Node * node_src, Node ** node_dest, Node * dest_parent, bool_t left);
int DifferSubTree(Node * node_src, Node ** node_dest, Node * dest_parent, Tree * dest_tree, bool_t left);

#endif //DIFFERENTIATOR_DIFFERENTIATOR_UTILS_H
