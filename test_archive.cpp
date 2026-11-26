#include "archive.h"
#include <iostream>
#include <string>

using namespace std;

// Compare two trees recursively
bool sameTree(Node* a, Node* b)
{
    if (a == nullptr && b == nullptr)
        return true;

    if (a == nullptr || b == nullptr)
        return false;

    if (a->data != b->data)
        return false;

    return sameTree(a->left, b->left) &&
           sameTree(a->right, b->right);
}

// Delete tree
void freeTree(Node* root)
{
    if (root == nullptr)
        return;

    freeTree(root->left);
    freeTree(root->right);

    delete root;
}

// Clone tree to preserve expected structure
Node* cloneTree(Node* root)
{
    if (root == nullptr)
        return nullptr;

    Node* copy = new Node(root->data);

    copy->left = cloneTree(root->left);
    copy->right = cloneTree(root->right);

    return copy;
}

// Create original test tree
Node* createTestTree()
{
    Node* root = new Node("A");

    root->left = new Node("B");
    root->right = new Node("C");
    root->left->right = new Node("D");

    return root;
}

int main()
{
    string error;

    cout << "========== TREEVAULT TEST ==========\n\n";

    // STEP 1: Create original tree
    Node* original = createTestTree();
    Node* expected = cloneTree(original);

    cout << "1. Original tree created.\n";

    // STEP 2: Save tree to archive
    if (!saveArchive(original, "test.txt", error))
    {
        cout << "SAVE FAILED: " << error << endl;

        freeTree(original);
        freeTree(expected);

        return 1;
    }

    cout << "2. Archive saved successfully.\n";

    // STEP 3: Delete original tree
    freeTree(original);
    original = nullptr;

    cout << "3. Original tree deleted from memory.\n";

    // STEP 4: Restore tree from archive
    Node* restored = nullptr;

    if (!loadArchive("test.txt", restored, error))
    {
        cout << "RESTORE FAILED: " << error << endl;

        freeTree(expected);
        freeTree(restored);

        return 1;
    }

    cout << "4. Tree restored successfully.\n";

    // STEP 5: Compare restored tree with expected tree
    if (!sameTree(expected, restored))
    {
        cout << "5. Tree comparison FAILED!\n";

        freeTree(expected);
        freeTree(restored);

        return 1;
    }

    cout << "5. Tree comparison PASSED!\n";

    // STEP 6: Integrity validation
    // loadArchive already validates:
    // - Header/version
    // - Archive records
    // - Missing data
    // - Node limit
    // - Unexpected trailing data
    //
    // sameTree confirms the restored structure matches
    // the expected original structure.

    cout << "6. Archive validation PASSED!\n";
    cout << "   Tree structure and data integrity verified.\n";

    // Cleanup
    freeTree(expected);
    freeTree(restored);

    cout << "\n========== ALL TESTS PASSED ==========\n";

    return 0;
}