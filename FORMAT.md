# TREEVAULT Archive Format

## 1. Overview
TREEVAULT stores a binary tree as plain text. The format is lossless: it records
every absent child, so left-only and right-only children are restored in the
correct place.

## 2. Header
Line 1 of every archive is:

    TREEVAULT_V1

## 3. Records
The tree is written in **preorder** (Root, Left, Right), one record per line:

- `N` marks a node. The next line is that node's label.
- `#` marks an absent (NULL) child.

Every node is followed by exactly two subtrees (left, then right), so the
structure can always be rebuilt unambiguously.

## 4. Example
Tree: A has left child B and right child C; B has only a right child D.

        A
       / \
      B   C
       \
        D

Archive:

    TREEVAULT_V1
    N
    A
    N
    B
    #
    N
    D
    #
    #
    N
    C
    #
    #

Reading it: A, then B's subtree (B, left is `#`, right is D with two `#`),
then C's subtree (C with two `#`).

Restored preorder: [A,B,D,C]. Restored inorder: [B,D,A,C].

## 5. Limits and Validation
- Maximum 2000 nodes (export and restore both enforce this).
- Labels must be a single line (no newline characters).
- Rejected with an error message:
  - missing or wrong header
  - truncated file (unexpected end of file, missing label)
  - unknown record (anything other than `N` or `#`)
  - extra data after the tree is complete

## 6. Recovery
Restore reads the header, rebuilds the tree recursively, and checks that no
data is left over. The tree is handed back only after it passes every check;
a partly built tree is discarded on failure.