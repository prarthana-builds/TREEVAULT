#include "tree.h"
#include "archive.h"
#include "views.h"
#include <fstream>
#include <iostream>
using namespace std;

static int passed = 0, failed = 0;
static void check(bool ok, const string& name) {
    cout << (ok ? "[PASS] " : "[FAIL] ") << name << "\n";
    ok ? passed++ : failed++;
}
static void write(const string& f, const string& content) { ofstream(f) << content; }

static bool roundTrip(Node* t, const string& name) {
    string err; TreeSnapshot before = takeSnapshot(t);
    bool saved = saveArchive(t, "rt.txt", err);
    deleteTree(t);
    Node* r = nullptr;
    bool loaded = saved && loadArchive("rt.txt", r, err);
    bool same = loaded && takeSnapshot(r).shape == before.shape &&
                takeSnapshot(r).preorder == before.preorder &&
                takeSnapshot(r).inorder == before.inorder;
    deleteTree(r);
    check(same, name);
    return same;
}

static void rejects(const string& name, const string& content, const string& expectText) {
    write("bad.txt", content);
    Node* r = nullptr; string err;
    bool rejected = !loadArchive("bad.txt", r, err) && r == nullptr;
    bool msgOk = err.find(expectText) != string::npos;
    cout << "       error: " << err << "\n";
    check(rejected && msgOk, name);
    deleteTree(r);
}

int main() {
    cout << "===== ROUND-TRIP TESTS =====\n";
    { Node* t = new Node("A"); t->left = new Node("B"); t->right = new Node("C");
      t->left->right = new Node("D"); roundTrip(t, "problem-statement example (B has right child D)"); }
    { Node* t = new Node("A"); t->left = new Node("B"); t->left->left = new Node("C");
      roundTrip(t, "left-skewed chain"); }
    { Node* t = new Node("A"); t->right = new Node("B"); t->right->right = new Node("C");
      roundTrip(t, "right-skewed chain"); }
    { Node* a = new Node("X"); a->left = new Node("Y"); Node* b = new Node("X"); b->right = new Node("Y");
      check(shapeSignature(a) != shapeSignature(b), "left-only and right-only trees have different signatures");
      deleteTree(a); deleteTree(b); }
    { roundTrip(new Node("ONLY"), "single node"); }
    { Node* r = nullptr; roundTrip(r, "empty tree"); }
    { Node* t = new Node("Museum Founder"); t->left = new Node("Mary Ann"); t->right = new Node("J. Smith");
      roundTrip(t, "labels containing spaces"); }
    { Node* t = new Node("0"); Node* c = t;
      for (int i = 1; i < 2000; i++) { c->right = new Node(to_string(i)); c = c->right; }
      roundTrip(t, "2000-node tree (maximum)"); }
    { Node* t = new Node("0"); Node* c = t;
      for (int i = 1; i < 2001; i++) { c->left = new Node(to_string(i)); c = c->left; }
      string err; check(!saveArchive(t, "big.txt", err), "2001-node tree refused on export");
      cout << "       error: " << err << "\n"; deleteTree(t); }

    cout << "\n===== DAMAGED-INPUT TESTS =====\n";
    rejects("missing header", "N\nA\n#\n#\n", "header");
    rejects("wrong version header", "TREEVAULT_V9\nN\nA\n#\n#\n", "header");
    rejects("truncated (missing right child)", "TREEVAULT_V1\nN\nA\nN\nB\n#\n#\n", "Incomplete");
    rejects("truncated (missing label)", "TREEVAULT_V1\nN\n", "missing node label");
    rejects("unknown record", "TREEVAULT_V1\nN\nA\nX\n#\n", "Invalid");
    rejects("trailing data", "TREEVAULT_V1\nN\nA\n#\n#\nN\nZ\n#\n#\n", "trailing");
    rejects("empty file", "", "header");
    { Node* r = nullptr; string err;
      check(!loadArchive("does_not_exist.txt", r, err), "nonexistent file rejected");
      cout << "       error: " << err << "\n"; }
    { Node* r = new Node("keep"); string err;
      check(!loadArchive("rt.txt", r, err), "restore refused when memory already holds a tree");
      deleteTree(r); }

    cout << "\nPassed: " << passed << "  Failed: " << failed << "\n";
    return failed ? 1 : 0;
}