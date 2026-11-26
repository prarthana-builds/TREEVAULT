#include "views.h"
#include <iostream>
using namespace std;

int countNodes(Node* r) { return r ? 1 + countNodes(r->left) + countNodes(r->right) : 0; }

static void pre(Node* r, string& s) {
    if (!r) return;
    s += (s.empty() ? "" : ",") + r->data;
    pre(r->left, s); pre(r->right, s);
}
static void in(Node* r, string& s) {
    if (!r) return;
    in(r->left, s);
    s += (s.empty() ? "" : ",") + r->data;
    in(r->right, s);
}
static void shape(Node* r, string& s) {
    if (!r) { s += "# "; return; }
    s += r->data + " ";
    shape(r->left, s); shape(r->right, s);
}

string preorderString(Node* r) { string s; pre(r, s); return "[" + s + "]"; }
string inorderString(Node* r)  { string s; in(r, s);  return "[" + s + "]"; }
string shapeSignature(Node* r) { string s; shape(r, s); return s; }

static void indented(Node* r, const string& prefix, const string& tag) {
    if (!r) { cout << prefix << tag << "(empty)\n"; return; }
    cout << prefix << tag << r->data << "\n";
    if (!r->left && !r->right) return;
    indented(r->left,  prefix + "    ", "L: ");
    indented(r->right, prefix + "    ", "R: ");
}

void printIndented(Node* root) {
    if (!root) { cout << "(empty tree)\n"; return; }
    indented(root, "", "ROOT: ");
}

TreeSnapshot takeSnapshot(Node* root) {
    TreeSnapshot t;
    t.nodes = countNodes(root);
    t.preorder = preorderString(root);
    t.inorder = inorderString(root);
    t.shape = shapeSignature(root);
    return t;
}

bool integrityReport(const TreeSnapshot& b, Node* restored) {
    TreeSnapshot a = takeSnapshot(restored);
    bool n = a.nodes == b.nodes, p = a.preorder == b.preorder,
         i = a.inorder == b.inorder, s = a.shape == b.shape;
    cout << "\n===== INTEGRITY REPORT =====\n";
    cout << "Node count : " << b.nodes << " vs " << a.nodes << (n ? "  MATCH\n" : "  MISMATCH\n");
    cout << "Preorder   : " << b.preorder << " vs " << a.preorder << (p ? "  MATCH\n" : "  MISMATCH\n");
    cout << "Inorder    : " << b.inorder << " vs " << a.inorder << (i ? "  MATCH\n" : "  MISMATCH\n");
    cout << "Shape (with missing branches): " << (s ? "MATCH\n" : "MISMATCH\n");
    bool ok = n && p && i && s;
    cout << "RESULT: " << (ok ? "PASS - restored tree is identical" : "FAIL - trees differ") << "\n";
    return ok;
}