#ifndef DIFFERENTIATOR_TEST_UTILS_H
#define DIFFERENTIATOR_TEST_UTILS_H

#include <stdio.h>
#include <string.h>
#include "../Common/data_types.h"
#include "../Common/err_codes.h"
#include "../TreeUtils/Node.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/** @file */

static const char * digraph_mask = "digraph Tree {\ngraph[bgcolor = cornsilk]\nnode[color = blue]\n";

const int BRACE_SPACE_NUM = 4;

int NodeOk(struct NodeStr * node);
int NodeTextDump(struct NodeStr * node);

int WriteDigraphFile(const char * filename, struct NodeStr * node);
int WriteNodeToDigraph(FILE * dump_file, struct NodeStr * node);
int WriteNodeIds(FILE * dump_file, struct NodeStr * node);

int WriteTreeStruct(const char * filename, struct NodeStr * node);
int WriteNodes(FILE * tree_struct, struct NodeStr * node);

#endif //DIFFERENTIATOR_TEST_UTILS_H
