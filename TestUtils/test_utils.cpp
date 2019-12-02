#include "test_utils.h"
#include "../TreeUtils/Node.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/** @file */

int NodeOk(Node * node) {

    if(node == nullptr){
        return ERR_NODE_OK_NODE;
    }

    if(node->tree == nullptr){
        return ERR_NODE_OK_TREE;
    }

    if(node->right){

        if(node->right->parent != node){
            return ERR_NODE_OK_PAR_RGHT;
        }

        if(node->type != OPERATOR){
            return ERR_NODE_OK_TYPE;
        }

    }

    if(node->left){

        if(node->left->parent != node){
            return ERR_NODE_OK_PAR_LEFT;
        }

        if(node->type != OPERATOR){
            return ERR_NODE_OK_TYPE;
        }

    }

    if(node->parent){

        if(node != node->parent->left && node != node->parent->right){
            return ERR_NODE_OK_PAR;
        }

        if(node->parent->type == VALUE || node->parent->type == VARIABLE){
            return ERR_NODE_OK_PAR_TYPE;
        }

    }

    if(node->type == NONE){
        return ERR_NODE_OK_NONE;
    }

    return 0;
}

int NodeTextDump(Node * node) {
    int err_code = 0;

    err_code = NodeOk(node);

    if(err_code == ERR_NODE_OK_NODE){
        printf("Node is nullptr.\n");
        return err_code;
    }

    printf("Node [%p]{\n" ,node);
    printf("   type = %d\n", node->type);
    printf("   val = %lg\n", node->val);
    printf("   tree [%p]\n", node->tree);
    if (node->tree){
        printf("      nodes_num = %zd\n", node->tree->nodes_num);
        printf("      max_nodes = %zd\n", node->tree->max_nodes);
        printf("      root [%p]\n", node->tree->root);
        if (node->tree->root){
            printf("      root type = %d\n", node->tree->root->type);
            printf("      root val = %lg\n", node->tree->root->val);
        }
    }
    printf("   }\n");
    printf("   left [%p]{\n", node->left);
    if (node->left){
        printf("      type = %d\n", node->left->type);
        printf("      val = %lg\n", node->left->val);
        printf("      parent [%p]\n", node->left->parent);
    }
    printf("   }\n");
    printf("   right [%p]{\n", node->right);
    if (node->right){
        printf("      type = %d\n", node->right->type);
        printf("      val = %lg\n", node->right->val);
        printf("      parent [%p]\n", node->right->parent);
    }
    printf("   }\n");
    printf("   parent [%p]{\n", node->parent);
    if (node->parent){
        printf("      type = %d\n", node->parent->type);
        printf("      val = %lg\n", node->parent->val);
        printf("      parent [%p]\n", node->parent->parent);
        printf("      left [%p]\n", node->parent->left);
        printf("      right [%p]\n", node->parent->right);
    }
    printf("   }\n}\n");

    return err_code;
}

int WriteDigraphFile(const char * filename, Node * node) {
    int err_code = 0;

    FILE * dump_file = fopen(filename, "w");

    fprintf(dump_file, "%s", digraph_mask);

    err_code = NodeOk(node);

    if (err_code == ERR_NODE_OK_NODE){
        printf("Node is nullptr.\n");
        return err_code;
    } else if (err_code){
        #ifndef NDEBUG
        err_code = NodeTextDump(node);
        #endif
        return err_code;
    }

    err_code = WriteNodeIds(dump_file, node);
    CHECK_ERROR;
    err_code = WriteNodeToDigraph(dump_file, node);
    CHECK_ERROR;

    fprintf(dump_file, "}");

    fclose(dump_file);

    return err_code;
}

int WriteNodeIds(FILE * dump_file, Node * node) {
    int err_code = 0;

    if(dump_file == nullptr){
        return 666;
    }

    if (node == nullptr){
        return 666;
    }
    if (node->type == OPERATOR){
        const char * oper_name = nullptr;
        err_code = GetOperName(node->val, &oper_name);
        if(err_code){
            return err_code;
        }
        fprintf(dump_file, "%zu [label=\"%s\"];\n", node->id, oper_name);
    } else if (node->type == VALUE) {
        fprintf(dump_file, "%zu [label=\"%lg\"];\n", node->id, node->val);
    } else {
        const char * var_name = nullptr;
        err_code = GetVarName(node->val, &var_name);
        if(err_code){
            return err_code;
        }
        fprintf(dump_file, "%zu [label=\"%s\"];\n", node->id, var_name);
    }

    if(node->left && node->right){
        err_code = WriteNodeIds(dump_file, node->left);
        if(err_code){
            return err_code;
        }
        err_code = WriteNodeIds(dump_file, node->right);
        if(err_code){
            return err_code;
        }
    } else if (node->left){
        err_code = WriteNodeIds(dump_file, node->left);
        if(err_code){
            return err_code;
        }
    } else if (node->right){
        err_code = WriteNodeIds(dump_file, node->right);
        if(err_code){
            return err_code;
        }
    }

    return err_code;
}

int WriteNodeToDigraph(FILE * dump_file, Node * node) {
    int err_code = 0;

    if (dump_file == nullptr){
        return 666;
    }

    if (node == nullptr){
        return 666;
    }

    fprintf(dump_file, "%zu", node->id);

    if(node->left && node->right){
        fprintf(dump_file, "->");
        err_code = WriteNodeToDigraph(dump_file, node->left);
        if (err_code){
            return err_code;
        }
        fprintf(dump_file, "%zu->", node->id);
        err_code = WriteNodeToDigraph(dump_file, node->right);
        if (err_code){
            return err_code;
        }
    } else if(node->left){
        fprintf(dump_file, "->");
        err_code = WriteNodeToDigraph(dump_file, node->left);
        if (err_code){
            return err_code;
        }
    } else if (node->right) {
        fprintf(dump_file, "->");
        err_code = WriteNodeToDigraph(dump_file, node->right);
        if (err_code){
            return err_code;
        }
    } else {
        fprintf(dump_file, "\n");
    }

    return err_code;
}

int WriteTreeStruct(const char * filename, Node * node) {
    int err_code = 0;
    FILE * tree_struct = nullptr;

    if (filename == nullptr){
        return ERR_WRT_TREE_FILE;
    }

    if (node == nullptr ){
        return ERR_WRT_TREE_NODE;
    }

    tree_struct = fopen(filename, "w");
    if (tree_struct == nullptr){
        return ERR_WRT_TREE_FOPEN;
    }

    err_code = WriteNodes(tree_struct, node);

    fclose(tree_struct);

    return err_code;
}

int WriteNodes(FILE * tree_struct, struct NodeStr * node) {
    int err_code = 0;
    static unsigned int count = 0;

    if (tree_struct == nullptr){
        return ERR_WRT_NODE_FILE;
    }

    if (node == nullptr ){
        return ERR_WRT_NODE_NODE;
    }

    fprintf(tree_struct, "%*s{", count*BRACE_SPACE_NUM, "");

    if (node->type == OPERATOR){
        const char * oper_name = nullptr;
        err_code = GetOperName(node->val, &oper_name);
        if(err_code){
            return err_code;
        }
        fprintf(tree_struct, "%s\n", oper_name);
    } else if (node->type == VALUE) {
        fprintf(tree_struct, "%lg\n", node->val);
    } else {
        const char * var_name = nullptr;
        err_code = GetVarName(node->val, &var_name);
        if(err_code){
            return err_code;
        }
        fprintf(tree_struct, "%s\n", var_name);
    }

    count++;

    if (node->left && node->right){
        err_code = WriteNodes(tree_struct, node->left);
        if (err_code){
            return err_code;
        }
        err_code = WriteNodes(tree_struct, node->right);
        if (err_code){
            return err_code;
        }
    } else if (node->left){
        err_code = WriteNodes(tree_struct, node->left);
        if (err_code){
            return err_code;
        }
        fprintf(tree_struct, "%*s{\n%*s}\n", count*BRACE_SPACE_NUM, "", count*BRACE_SPACE_NUM, "");
    } else if (node->right) {
        fprintf(tree_struct, "%*s{\n%*s}\n", count*BRACE_SPACE_NUM, "", count*BRACE_SPACE_NUM, "");
        err_code = WriteNodes(tree_struct, node->right);
        if (err_code){
            return err_code;
        }
    }

    count--;
    fprintf(tree_struct, "%*s}\n", count*BRACE_SPACE_NUM, "");

    return err_code;
}

int WriteTexStruct(const char * filename, struct NodeStr  * node) {
    int err_code = 0;
    FILE * tree_struct = nullptr;

    if (filename == nullptr){
        return ERR_WRT_TEX_FILE;
    }

    if (node == nullptr ){
        return ERR_WRT_TEX_NODE;
    }

    tree_struct = fopen(filename, "w");
    if (tree_struct == nullptr){
        return ERR_WRT_TEX_FOPEN;
    }

    fprintf(tree_struct, "%s", tex_start_mask);

    err_code = WriteTexNodes(tree_struct, node, false);
    CHECK_ERROR;

    fprintf(tree_struct, "%s", tex_end_mask);
    fclose(tree_struct);

    return err_code;
}

int WriteTexNodes(FILE * tree_struct, struct NodeStr * node, bool need_bracket) {
    int err_code = 0;
    bool child_brack = false;

    if (tree_struct == nullptr){
        return ERR_WRT_TEX_NODE_FILE;
    }

    if (node == nullptr ){
        return ERR_WRT_TEX_NODE_NODE;
    }

    if(node->type == OPERATOR && node->val == OP_DIV){

        if (node->left && node->right) {
            if(need_bracket){
                fprintf(tree_struct, "(");
            }
            fprintf(tree_struct, "\\frac{");
            err_code = WriteTexNodes(tree_struct, node->left, child_brack);
            CHECK_ERROR;
            fprintf(tree_struct, "}{");
            err_code = WriteTexNodes(tree_struct, node->right, child_brack);
            CHECK_ERROR;
            fprintf(tree_struct, "}");
            if(need_bracket){
                fprintf(tree_struct, ")");
            }
        } else {
            return ERR_WRT_TEX_NODE_NOT_OP_CHLD;
        }

    } else {

        if (node->left && node->right) {
            if(need_bracket){
                fprintf(tree_struct, "(");
            }
            if(oper[(int) node->val].priority < oper[(int) node->left->val].priority){
                child_brack = true;
            } else {
                child_brack = false;
            }
            err_code = WriteTexNodes(tree_struct, node->left, child_brack);
            CHECK_ERROR;
            if (node->type == OPERATOR) {
                if(node->val != OP_MUL) {
                    const char *oper_name = nullptr;
                    err_code = GetOperName(node->val, &oper_name);
                    CHECK_ERROR;
                    fprintf(tree_struct, "%s", oper_name);
                } else {
                    fprintf(tree_struct, " \\cdot ");
                }
            } else {
                return ERR_WRT_TEX_NODE_NOT_OP_CHLD;
            }
            if(oper[(int) node->val].priority < oper[(int) node->right->val].priority){
                child_brack = true;
            } else {
                child_brack = false;
            }
            err_code = WriteTexNodes(tree_struct, node->right, child_brack);
            CHECK_ERROR;
            if(need_bracket){
                fprintf(tree_struct, ")");
            }
        } else if (node->left) {
            if(need_bracket){
                fprintf(tree_struct, "(");
            }
            if(oper[(int) node->val].priority < oper[(int) node->left->val].priority){
                child_brack = true;
            } else {
                child_brack = false;
            }
            if (node->type == OPERATOR) {
                const char *oper_name = nullptr;
                err_code = GetOperName(node->val, &oper_name);
                CHECK_ERROR;
                fprintf(tree_struct, "%s", oper_name);
            } else {
                return ERR_WRT_TEX_NODE_NOT_OP_CHLD;
            }
            err_code = WriteTexNodes(tree_struct, node->left, child_brack);
            CHECK_ERROR;
            if(need_bracket){
                fprintf(tree_struct, ")");
            }
        } else {
            if (node->type == VALUE) {
                fprintf(tree_struct, "%lg", node->val);
            } else if (node->type == VARIABLE) {
                const char *var_name = nullptr;
                err_code = GetVarName(node->val, &var_name);
                CHECK_ERROR;
                fprintf(tree_struct, "%s", var_name);
            } else {
                return ERR_WRT_TEX_NODE_WRONG_TYPE;
            }
        }
    }

    return err_code;
}
