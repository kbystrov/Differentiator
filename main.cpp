#include <iostream>
#include "TreeUtils/Tree.h"
#include "DifferentiatorUtils/differentiator_utils.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/** @file */

int main() {
    std::cout << "Hello, World!" << std::endl;
    int err_code = 0;
    Tree * tree = nullptr;
    Tree * diff_tree = nullptr;

    err_code = CreateTree(&tree, TREE_NODES);
    if(err_code){
        return err_code;
    }

    err_code = CreateTree(&diff_tree, TREE_NODES);
    if(err_code){
        return err_code;
    }
    //! ++++++++++++++ Creating digraph from Tree struct test ++++++++++++++++++++++++++++
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

    AddChild(tree->root->left->right, OP_MUL, OPERATOR, 0);
    /*if(err_code){
        return err_code;
    }
    */

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

    //! ++++++++++++++ Creating Tree struct from special notation file test ++++++++++++++++++++++++++++
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

    //! ++++++++++++++ Differentiator test ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    err_code = CpyNode(tree->root, &diff_tree->root, diff_tree);
    if(err_code){
        return err_code;
    }
    /*
    err_code = DifferSubTree(tree->root->left, &diff_tree->root->left, diff_tree->root, diff_tree, 1);
    if(err_code){
        return err_code;
    }

    err_code = DifferSubTree(tree->root->right, &diff_tree->root->right, diff_tree->root, diff_tree, 0);
    if(err_code){
        return err_code;
    }
    */

    err_code = CopySubTree(tree->root->left, &diff_tree->root->left, diff_tree->root, 1);
    if(err_code){
        return err_code;
    }

    err_code = CopySubTree(tree->root->right, &diff_tree->root->right, diff_tree->root, 0);
    if(err_code){
        return err_code;
    }

    err_code = WriteDigraphFile("IOFiles/diff_test.txt", diff_tree->root);
    if(err_code){
        return err_code;
    }
    err_code = WriteTreeStruct("IOFiles/diff_struct.txt", diff_tree->root);
    if(err_code){
        return err_code;
    }

    system("dot -Tpng IOFiles/diff_test.txt -o IOFiles/diff_test.png");

    DeleteTree(tree);
    DeleteTree(diff_tree);

    return err_code;
}