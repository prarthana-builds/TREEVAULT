
#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <string>
#include <queue>

using namespace std;

// Binary Tree Node
struct Node {
    string data;
    Node* left;
    Node* right;

    Node(string value) {
        data = value;
        left = nullptr;
        right = nullptr;
    }
};

// Function declarations
Node* createTree();
void displayTree(Node* root);

void preorder(Node* root);
void inorder(Node* root);
void postorder(Node* root);

bool identical(Node* a, Node* b);

void deleteTree(Node*& root);


#endif
