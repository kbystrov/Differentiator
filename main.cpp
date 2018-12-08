#include <iostream>
#include "TreeUtils/Tree.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/** @file */

int main() {
    std::cout << "Hello, World!" << std::endl;
    int err_code = 0;
    Tree * tree = nullptr;

    err_code = CreateTree(&tree, TREE_NODES);
    if(err_code){
        return err_code;
    }

    err_code = CreateNode(&tree->root, OP_DIV, OPERATOR, tree);
    if(err_code){
        return err_code;
    }

    err_code = AddChild(tree->root, OP_SUB, OPERATOR, 1);
    if(err_code){
        return err_code;
    }

    err_code = AddChild(tree->root, OP_ADD, OPERATOR, 0);
    if(err_code){
        return err_code;
    }

    err_code = AddChild(tree->root->left, 15, VALUE, 1);
    if(err_code){
        return err_code;
    }

    err_code = AddChild(tree->root->left, 7, VALUE, 0);
    if(err_code){
        return err_code;
    }

    err_code = AddChild(tree->root->right, VAR_x, VARIABLE, 1);
    if(err_code){
        return err_code;
    }

    err_code = AddChild(tree->root->right, 3, VALUE, 0);
    if(err_code){
        return err_code;
    }

    err_code = AddChild(tree->root->left->right, OP_MUL, OPERATOR, 0);
    if(err_code){
        return err_code;
    }


    err_code = WriteDigraphFile("IOFiles/diag_test.txt", tree->root);
    if(err_code){
        return err_code;
    }
    err_code = WriteTreeStruct("IOFiles/tree_struct.txt", tree->root);
    if(err_code){
        return err_code;
    }

    system("dot -Tpng IOFiles/diag_test.txt -o IOFiles/diag_test.png");

    DeleteTree(tree);

    err_code = CreateTreeFromFile(&tree, "IOFiles/tree_struct.txt");
    if(err_code){
        return err_code;
    }

    err_code = WriteDigraphFile("IOFiles/write_test.txt", tree->root);
    if(err_code){
        return err_code;
    }
    err_code = WriteTreeStruct("IOFiles/write_struct.txt", tree->root);
    if(err_code){
        return err_code;
    }

    system("dot -Tpng IOFiles/write_test.txt -o IOFiles/write_test.png");

    DeleteTree(tree);

    return err_code;
}