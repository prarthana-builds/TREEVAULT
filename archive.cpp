#include "archive.h"

#include <fstream>
#include <sstream>

// Recursive helper function
bool serializeHelper(Node* root, ostream& out,
                    int& nodeCount, string& error)
{
    // Base case: missing node
    if (root == nullptr)
    {
        out << "#\n";
        return true;
    }

    // Enforce maximum node limit
    if (nodeCount >= 2000)
    {
        error = "Tree exceeds 2000 nodes.";
        return false;
    }

    // Reject labels containing newline characters
    if (root->data.find('\n') != string::npos ||
        root->data.find('\r') != string::npos)
    {
        error = "Node labels must be single-line strings.";
        return false;
    }

    // Record this node
    nodeCount++;

    out << "N\n";
    out << root->data << "\n";

    // Serialize left subtree
    if (!serializeHelper(root->left, out, nodeCount, error))
        return false;

    // Serialize right subtree
    if (!serializeHelper(root->right, out, nodeCount, error))
        return false;

    return true;
}


// Save tree to archive file
bool saveArchive(Node* root,
                 const string& filename,
                 string& error)
{
    error.clear();

    // Temporary buffer: build archive before writing to file
    stringstream buffer;

    buffer << "TREEVAULT_V1\n";

    int nodeCount = 0;

    // Serialize the complete tree
    if (!serializeHelper(root, buffer, nodeCount, error))
    {
        return false;
    }

    // Open file for writing
    ofstream file(filename);

    if (!file.is_open())
    {
        error = "Unable to open archive file.";
        return false;
    }

    // Write generated archive into file
    file << buffer.str();

    if (!file.good())
    {
        error = "Error while writing archive file.";
        file.close();
        return false;
    }

    file.close();

    return true;
}
// Recursive helper for rebuilding a tree
bool deserializeHelper(istream& in,
                      Node*& root,
                      int& nodeCount,
                      string& error)
{
    string record;

    // Read the next record
    if (!getline(in, record))
    {
        error = "Incomplete archive: unexpected end of file.";
        return false;
    }

    // NULL child
    if (record == "#")
    {
        root = nullptr;
        return true;
    }

    // Invalid record
    if (record != "N")
    {
        error = "Invalid archive record.";
        return false;
    }

    // Enforce node limit
    if (nodeCount >= 2000)
    {
        error = "Archive exceeds 2000 nodes.";
        return false;
    }

    // Read node label
    string label;

    if (!getline(in, label))
    {
        error = "Incomplete archive: missing node label.";
        return false;
    }

    // Create node
    root = new Node(label);
    nodeCount++;

    // Restore left child
    if (!deserializeHelper(in, root->left, nodeCount, error))
        return false;

    // Restore right child
    if (!deserializeHelper(in, root->right, nodeCount, error))
        return false;

    return true;
}
// Safely destroy a partially restored tree
void cleanupRestoredTree(Node* root)
{
    if (root == nullptr)
        return;

    cleanupRestoredTree(root->left);
    cleanupRestoredTree(root->right);

    delete root;
}
bool loadArchive(const string& filename,
                 Node*& root,
                 string& error)
{
    error.clear();

    // Require an empty output pointer to avoid ownership ambiguity
    if (root != nullptr)
    {
        error = "Output root must be nullptr before restoration.";
        return false;
    }

    // Open archive
    ifstream file(filename);

    if (!file.is_open())
    {
        error = "Unable to open archive file.";
        return false;
    }

    // Validate archive header
    string header;

    if (!getline(file, header) || header != "TREEVAULT_V1")
    {
        error = "Invalid or missing archive header.";
        return false;
    }

    // Build into a temporary tree
    Node* restored = nullptr;
    int nodeCount = 0;

    if (!deserializeHelper(file, restored, nodeCount, error))
    {
        cleanupRestoredTree(restored);
        return false;
    }

    // Reject unexpected trailing data
    string extra;

    if (getline(file, extra))
    {
        cleanupRestoredTree(restored);
        error = "Unexpected trailing data in archive.";
        return false;
    }

    if (file.bad())
    {
        cleanupRestoredTree(restored);
        error = "Error reading archive file.";
        return false;
    }

    // Transfer ownership to caller only after full validation
    root = restored;

    return true;
}