#include "tree.h"
#include "archive.h"
#include "views.h"
#include <iostream>
#include <limits>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

static Node* g_tree = nullptr;
static TreeSnapshot g_before;
static bool g_haveSnapshot = false;
static const string DEFAULT_FILE = "museum_archive.txt";

static string askFile() {
    cout << "Archive filename [" << DEFAULT_FILE << "]: ";
    string f; getline(cin >> ws, f);
    return f.empty() ? DEFAULT_FILE : f;
}

static void showTree(const string& title) {
    cout << "\n--- " << title << " ---\n";
    printIndented(g_tree);
    cout << "Preorder: " << preorderString(g_tree) << "\n";
    cout << "Inorder : " << inorderString(g_tree) << "\n";
    cout << "Nodes   : " << countNodes(g_tree) << "\n";
}

static void loadSample() {
    deleteTree(g_tree);
    g_tree = new Node("A");
    g_tree->left = new Node("B");
    g_tree->right = new Node("C");
    g_tree->left->right = new Node("D");
    cout << "Sample tree loaded (A; left B, right C; B's right child D).\n";
}

static void doExport() {
    if (!g_tree) { cout << "Nothing to export. Create a tree first.\n"; return; }
    string f = askFile(), err;
    g_before = takeSnapshot(g_tree);
    if (saveArchive(g_tree, f, err)) { g_haveSnapshot = true; cout << "Exported to " << f << "\n"; }
    else cout << "EXPORT FAILED: " << err << "\n";
}

static void doClear() {
    deleteTree(g_tree);
    cout << "In-memory tree cleared.\n";
}

static bool doRestore() {
    if (g_tree) { cout << "Memory still holds a tree. Clear it first (option 6).\n"; return false; }
    string f = askFile(), err;
    Node* r = nullptr;
    if (!loadArchive(f, r, err)) { cout << "RESTORE REJECTED: " << err << "\n"; return false; }
    g_tree = r;
    cout << "Restored from " << f << "\n";
    return true;
}

static void demo() {
    cout << "\n=== DEMO: export -> clear -> restore -> verify ===\n";
    loadSample();
    showTree("ORIGINAL");
    g_before = takeSnapshot(g_tree); g_haveSnapshot = true;
    string err;
    if (!saveArchive(g_tree, "demo_archive.txt", err)) { cout << err << "\n"; return; }
    cout << "\nExported to demo_archive.txt\n";
    doClear();
    Node* r = nullptr;
    if (!loadArchive("demo_archive.txt", r, err)) { cout << "Restore failed: " << err << "\n"; return; }
    g_tree = r;
    showTree("RESTORED");
    integrityReport(g_before, g_tree);
    cout << "\n=== DAMAGED EXPORT (last record removed) ===\n";
    {
        ifstream in("demo_archive.txt"); vector<string> lines; string l;
        while (getline(in, l)) lines.push_back(l);
        ofstream out("damaged_archive.txt");
        for (size_t i = 0; i + 1 < lines.size(); i++) out << lines[i] << "\n";
    }
    Node* bad = nullptr;
    if (!loadArchive("damaged_archive.txt", bad, err)) cout << "REJECTED as expected: " << err << "\n";
    else { cout << "ERROR: damaged file was accepted!\n"; deleteTree(bad); }
}

int main() {
    int choice = -1;
    while (choice != 0) {
        cout << "\n====== TREEVAULT ======\n"
             << "1. Create tree (level-order, # = missing)\n"
             << "2. Load sample tree\n"
             << "3. Display tree + traversals\n"
             << "4. Export tree to file\n"
             << "5. Show archive file format notes\n"
             << "6. Clear in-memory tree\n"
             << "7. Restore tree from file\n"
             << "8. Integrity report (before vs restored)\n"
             << "9. Run full demo\n"
             << "0. Exit\nChoice: ";
        if (!(cin >> choice)) {
            if (cin.eof()) break;
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); choice = -1;
        }
        switch (choice) {
            case 1: { Node* t = createTree(); if (t) { deleteTree(g_tree); g_tree = t; } break; }
            case 2: loadSample(); break;
            case 3: showTree("CURRENT TREE"); break;
            case 4: doExport(); break;
            case 5: cout << "Line 1: TREEVAULT_V1\nThen preorder: 'N' + label line for a node, '#' for an absent child.\n"; break;
            case 6: doClear(); break;
            case 7: if (doRestore()) showTree("RESTORED"); break;
            case 8:
                if (!g_haveSnapshot) cout << "Export a tree first.\n";
                else if (!g_tree) cout << "Restore a tree first.\n";
                else integrityReport(g_before, g_tree);
                break;
            case 9: demo(); break;
            case 0: break;
            default: cout << "Invalid choice.\n";
        }
    }
    deleteTree(g_tree);
    return 0;
}