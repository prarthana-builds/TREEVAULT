\# TREEVAULT Archive Format



\## 1. Overview

TREEVAULT uses a text-based archive format to store and restore binary trees. The archive preserves the tree structure and node data.



\## 2. Header

Every archive begins with the version header:



`TREEVAULT\_V1`



This header is used to verify the archive format and version.



\## 3. Serialization Format

The tree is serialized using \*\*Preorder Traversal\*\* (Root → Left → Right).



\- `N` represents a node.

\- `#` represents a NULL (empty) child.

\- Node data is stored after the `N` marker.



\## 4. Example



For the binary tree:



```text

&#x20;     A

&#x20;    / \\

&#x20;   B   C

&#x20;  /

&#x20; D

```



The serialized representation is:



```text

TREEVAULT\_V1

N

A

N

B

N

D

\#

\#

\#

N

C

\#

\#

```



\## 5. Limits and Validation



\- Maximum nodes allowed: \*\*2000\*\*

\- Invalid or missing headers are rejected.

\- Malformed archive data is rejected.

\- Missing node data is rejected.

\- Extra trailing data is rejected.

\- Node labels containing newline characters are not supported.

\- Partial trees are cleaned up if restoration fails.



\## 6. Recovery



During recovery, TREEVAULT reads the header, validates the archive, reconstructs the tree recursively, and restores the root only after successful validation.



