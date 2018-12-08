#ifndef DIFFERENTIATOR_TREE_H
#define DIFFERENTIATOR_TREE_H

#include "string.h"
#include "ctype.h"
#include "../Common/data_types.h"
#include "Node.h"
#include "../ReadWriteUtils/read_write_utils.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/** @file */

const size_t TREE_NODES = 100;

typedef struct TreeStr Tree;

struct TreeStr{
    struct NodeStr * root;
    tree_size_t nodes_num;
    tree_size_t  max_nodes;
};

int CreateTree(Tree ** tree, tree_size_t max_nodes);
int AddRootFromFile(Tree * tree, char * buf, char ** ret_pos);
int AddNodeToTree(char * buf, struct NodeStr * parent, size_t par_level, char ** ret_pos);
int DeleteTree(Tree * tree);

int CreateTreeFromFile(Tree ** tree, const char * filename);

#endif //DIFFERENTIATOR_TREE_H
