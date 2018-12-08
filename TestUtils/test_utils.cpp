#include "test_utils.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/** @file */

int NodeOk(Node * node) {

    if(node == nullptr){
        return ERR_NODE_OK_NODE;
    }

    if(node->tree == nullptr){
        return ERR_NODE_OK_TREE;
    }

    if(node->right && node->right->parent != node){
        return ERR_NODE_OK_PAR_RGHT;
    }

    if(node->left && node->left->parent != node){
        return ERR_NODE_OK_PAR_LEFT;
    }

    if(node->parent && node != node->parent->left && node != node->parent->right){
        return ERR_NODE_OK_PAR;
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
        err_code = NodeTextDump(node);
    }

    WriteNodeToDigraph(dump_file, node);

    fprintf(dump_file, "}");

    fclose(dump_file);

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

    if (node->type == OPERATOR){
        const char * oper_name = nullptr;
        err_code = GetOperName(node->val, &oper_name);
        if(err_code){
            return err_code;
        }
        fprintf(dump_file, "\"%s\"", oper_name);
    } else if (node->type == VALUE) {
        fprintf(dump_file, "%lg", node->val);
    } else {
        const char * var_name = nullptr;
        err_code = GetVarName(node->val, &var_name);
        if(err_code){
            return err_code;
        }
        fprintf(dump_file, "%s", var_name);
    }

    if(node->left && node->right){
        fprintf(dump_file, "->");
        WriteNodeToDigraph(dump_file, node->left);
        if (node->type == OPERATOR){
            const char * oper_name = nullptr;
            err_code = GetOperName(node->val, &oper_name);
            if(err_code){
                return err_code;
            }
            fprintf(dump_file, "\"%s\"->", oper_name);
        } else if (node->type == VALUE) {
            fprintf(dump_file, "%lg->", node->val);
        } else {
            const char * var_name = nullptr;
            err_code = GetVarName(node->val, &var_name);
            if(err_code){
                return err_code;
            }
            fprintf(dump_file, "\"%s\"->", var_name);
        }
        WriteNodeToDigraph(dump_file, node->right);
    } else if(node->left){
        fprintf(dump_file, "->");
        WriteNodeToDigraph(dump_file, node->left);
    } else if (node->right) {
        fprintf(dump_file, "->");
        WriteNodeToDigraph(dump_file, node->right);
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
