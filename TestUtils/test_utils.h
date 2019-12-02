#ifndef DIFFERENTIATOR_TEST_UTILS_H
#define DIFFERENTIATOR_TEST_UTILS_H

#include <stdio.h>
#include <string.h>
#include "../Common/data_types.h"
#include "../Common/err_codes.h"
#include "../TreeUtils/Node.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/** @file */

#ifndef NDEBUG
#define CheckNode(node) \
    err_code = NodeOk(node); \
    if(err_code){ \
        err_code = NodeTextDump(node); \
        return err_code; \
    }
#else
#define CheckNode(node) \
    err_code = NodeOk(node); \
    if(err_code){ \
        return err_code; \
    }
#endif

static const char * digraph_mask = "digraph Tree {\ngraph[bgcolor = cornsilk]\nnode[color = blue]\n";

static const char * tex_start_mask = "\\documentclass{book}\n\n\\usepackage[utf8]{inputenc}\n\\usepackage[russian]{babel}\n\n\\begin{document}\n\n\\chapter{Дифференцирование уравнений}\n\n$$";

static const char * tex_end_mask = "$$\n\n\\end{document}";

const int BRACE_SPACE_NUM = 4;

int NodeOk(struct NodeStr * node);
int NodeTextDump(struct NodeStr * node);

int WriteDigraphFile(const char * filename, struct NodeStr * node);
int WriteNodeToDigraph(FILE * dump_file, struct NodeStr * node);
int WriteNodeIds(FILE * dump_file, struct NodeStr * node);

int WriteTreeStruct(const char * filename, struct NodeStr * node);
int WriteNodes(FILE * tree_struct, struct NodeStr * node);

int WriteTexStruct(const char * filename, struct NodeStr * node);
int WriteTexNodes(FILE * tree_struct, struct NodeStr * node, bool need_bracket);

#endif //DIFFERENTIATOR_TEST_UTILS_H
