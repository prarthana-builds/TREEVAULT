

#include "tree.h"
#include <sstream>
#include <vector>

// Helper function for cleaning up a partially
// constructed tree if the input is invalid.
void cleanup(Node* root) {
    if (root == nullptr)
        return;

    cleanup(root->left);
    cleanup(root->right);

    delete root;
}

// Construct a binary tree using level order input.
Node* createTree() {

    string line;
    vector<string> tokens;

    cout << "Enter tree in level-order format:\n";
    cout << "Use # for missing children.\n";

    getline(cin >> ws, line);

    stringstream ss(line);
    string value;

    while (ss >> value) {
        tokens.push_back(value);
    }

    // Empty input
    if (tokens.empty()) {
        cout << "Error: Empty input.\n";
        return nullptr;
    }

    // Empty tree
    if (tokens[0] == "#") {
        if (tokens.size() == 1)
            return nullptr;

        cout << "Error: Invalid tree input.\n";
        return nullptr;
    }

    // Create root node
    Node* root = new Node(tokens[0]);

    queue<Node*> q;
    q.push(root);

    int index = 1;
    int nodeCount = 1;

    // Process nodes level by level
    while (!q.empty()) {

        Node* current = q.front();
        q.pop();

        // Both child positions must be provided
        if (index + 1 >= static_cast<int>(tokens.size())) {
            cout << "Error: Incomplete tree input.\n";
            cleanup(root);
            return nullptr;
        }

        // Process left child
        if (tokens[index] != "#") {

            if (nodeCount >= 2000) {
                cout << "Error: Node limit exceeded.\n";
                cleanup(root);
                return nullptr;
            }

            current->left = new Node(tokens[index]);
            q.push(current->left);
            nodeCount++;
        }

        index++;

        // Process right child
        if (tokens[index] != "#") {

            if (nodeCount >= 2000) {
                cout << "Error: Node limit exceeded.\n";
                cleanup(root);
                return nullptr;
            }

            current->right = new Node(tokens[index]);
            q.push(current->right);
            nodeCount++;
        }

        index++;
    }

    // Reject unexpected extra input
    if (index != static_cast<int>(tokens.size())) {
        cout << "Error: Extra input tokens found.\n";
        cleanup(root);
        return nullptr;
    }

    cout << "Tree created successfully!\n";

    return root;
}





// Main display function

// Helper function for tree visualization
void displayTreeHelper(Node* root, int space) {

    // Base condition
    if (root == nullptr)
        return;

    // Increase indentation for each level
    space += 5;

    // Display right subtree first
    displayTreeHelper(root->right, space);

    // Print current node with indentation
    cout << endl;

    for (int i = 5; i < space; i++) {
        cout << " ";
    }

    cout << root->data << endl;

    // Display left subtree
    displayTreeHelper(root->left, space);
}


// Main display function
void displayTree(Node* root) {

    if (root == nullptr) {
        cout << "Tree is empty.\n";
        return;
    }

    cout << "\nTREE STRUCTURE:\n";

    displayTreeHelper(root, 0);
}
// Preorder Traversal: Root → Left → Right
void preorder(Node* root) {

    if (root == nullptr)
        return;

    cout << root->data << " ";

    preorder(root->left);

    preorder(root->right);
}

// Inorder Traversal: Left -> Root -> Right
void inorder(Node* root) {

    if (root == nullptr)
        return;

    inorder(root->left);

    cout << root->data << " ";

    inorder(root->right);
}


// Postorder Traversal: Left -> Right -> Root
void postorder(Node* root) {

    if (root == nullptr)
        return;

    postorder(root->left);

    postorder(root->right);

    cout << root->data << " ";
}

// Check whether two binary trees are structurally identical
bool identical(Node* a, Node* b) {

    // Both trees are empty
    if (a == nullptr && b == nullptr)
        return true;

    // Only one tree is empty
    if (a == nullptr || b == nullptr)
        return false;

    // Compare node data and both subtrees
    return (a->data == b->data) &&
           identical(a->left, b->left) &&
           identical(a->right, b->right);
}
void deleteTree(Node*& root) {

    if (root == nullptr)
        return;

    deleteTree(root->left);
    deleteTree(root->right);

    delete root;
    root = nullptr;
}


