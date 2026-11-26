#ifndef VIEWS_H
#define VIEWS_H
#include "tree.h"
#include <string>
#include <vector>

struct TreeSnapshot {
    int nodes = 0;
    std::string preorder;
    std::string inorder;
    std::string shape;   // preorder with '#' for every absent child
};

int  countNodes(Node* root);
std::string preorderString(Node* root);
std::string inorderString(Node* root);
std::string shapeSignature(Node* root);
void printIndented(Node* root);
TreeSnapshot takeSnapshot(Node* root);
bool integrityReport(const TreeSnapshot& before, Node* restored);
#endif