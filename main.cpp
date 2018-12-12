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
    CHECK_ERROR;

    err_code = CreateTree(&diff_tree, TREE_NODES);
    CHECK_ERROR;
    //! ++++++++++++++ Creating digraph from Tree struct test ++++++++++++++++++++++++++++
    err_code = CreateNode(&tree->root, OP_ARCCTAN, OPERATOR, tree);
    CHECK_ERROR;

    //err_code = AddChild(tree->root, VAR_x, VARIABLE, 1);
    //CHECK_ERROR;

    err_code = AddChild(tree->root, OP_MUL, OPERATOR, 1);
    CHECK_ERROR;

    err_code = AddChild(tree->root->left, VAR_x, VARIABLE, 1);
    CHECK_ERROR;

    err_code = AddChild(tree->root->left, VAR_x, VARIABLE,  0);
    CHECK_ERROR;
    /*
    err_code = AddChild(tree->root->left, VAR_x, VARIABLE, 1);
    CHECK_ERROR;

    err_code = AddChild(tree->root->left, 3, VALUE, 0);
    CHECK_ERROR;
    */
    /*
    err_code = AddChild(tree->root, OP_EXP, OPERATOR, 1);
    CHECK_ERROR;

    err_code = AddChild(tree->root, OP_MUL, OPERATOR, 0);
    CHECK_ERROR;

    err_code = AddChild(tree->root->left, VAR_x, VARIABLE, 1);
    CHECK_ERROR;

    //err_code = AddChild(tree->root->left, 7, VALUE, 0);
    //CHECK_ERROR;

    err_code = AddChild(tree->root->right, VAR_x, VARIABLE, 1);
    CHECK_ERROR;

    err_code = AddChild(tree->root->right, VAR_x, VARIABLE, 0);
    CHECK_ERROR;

    AddChild(tree->root->left->right, OP_MUL, OPERATOR, 0);
    */
    err_code = WriteDigraphFile("IOFiles/diag_test.txt", tree->root);
    CHECK_ERROR;
    err_code = WriteTreeStruct("IOFiles/tree_struct.txt", tree->root);
    CHECK_ERROR;

    system("dot -Tpng IOFiles/diag_test.txt -o IOFiles/diag_test.png");

    DeleteTree(tree);

    //! ++++++++++++++ Creating Tree struct from special notation file test ++++++++++++++++++++++++++++
    err_code = CreateTreeFromFile(&tree, "IOFiles/tree_struct.txt");
    CHECK_ERROR;

    err_code = WriteDigraphFile("IOFiles/write_test.txt", tree->root);
    CHECK_ERROR;
    err_code = WriteTreeStruct("IOFiles/write_struct.txt", tree->root);
    CHECK_ERROR;

    system("dot -Tpng IOFiles/write_test.txt -o IOFiles/write_test.png");

    //! ++++++++++++++ Differentiator test ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    //err_code = DifferTreeRoot(tree->root, diff_tree);
    //CHECK_ERROR;
    err_code = DifferSubTree(tree->root, &diff_tree->root, NULL, diff_tree, 1);
    CHECK_ERROR;

    err_code = WriteDigraphFile("IOFiles/diff_test.txt", diff_tree->root);
    CHECK_ERROR;
    err_code = WriteTreeStruct("IOFiles/diff_struct.txt", diff_tree->root);
    CHECK_ERROR;

    system("dot -Tpng IOFiles/diff_test.txt -o IOFiles/diff_test.png");

    DeleteTree(tree);
    DeleteTree(diff_tree);

    return err_code;
}