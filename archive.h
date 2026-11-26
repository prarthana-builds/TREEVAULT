#ifndef ARCHIVE_H
#define ARCHIVE_H

#include "tree.h"
#include <string>

bool saveArchive(Node* root,
                 const string& filename,
                 string& error);

bool loadArchive(const string& filename,
                 Node*& root,
                 string& error);

#endif