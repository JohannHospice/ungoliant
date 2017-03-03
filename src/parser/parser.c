#include <stdio.h>
#include <curl.h>
#include <tidy.h>
#include <tidybuffio.h>
#include <string.h>
#include "../tools.h"
#define SIZEOF_TIDYNODE sizeof(TidyNode)

int queryNodeByDoc(TidyNode *buffer, int size, TidyDoc doc, TidyNode tnod, char *target) {
    TidyNode child;
    //erreur dans les parages
    for (child = tidyGetChild(tnod); child; child = tidyGetNext(child)) {
        const char *name = tidyNodeGetName(child);
        if (name && strcmp(name, target) == 0) {
            int sizeofBuffer = sizeof(buffer);
            if (sizeofBuffer < sizeofBuffer + SIZEOF_TIDYNODE)
                buffer = realloc(buffer, (size + 1) * SIZEOF_TIDYNODE);
            buffer[size] = child;
            size++;
        }
        size = queryNodeByDoc(buffer, size, doc, child, target);
    }
    return size;
}

char *queryAttrByNode(TidyNode node, char *target) {
    TidyAttr attr;
    char *value;
    for (attr = tidyAttrFirst(node); attr; attr = tidyAttrNext(attr)) {
        const char *name = tidyAttrName(attr);
        if (name && strcmp(name, target) == 0)
            value = (char *) tidyAttrValue(attr);
    }
    return value;
}

char **queryAttrByAllNodes(char **buffers, TidyNode *nodes, int nodes_size, char *target) {
    int i = 0;
    for (i = 0; i < nodes_size; ++i)
        buffers[i] = queryAttrByNode(nodes[i], target);
    return buffers;
}

int parseAux(TidyDoc tdoc) {
    int err = tidyCleanAndRepair(tdoc);
    if (err >= 0)
        err = tidyRunDiagnostics(tdoc);
    return err;
}

int parseBuffer(TidyDoc tdoc, TidyBuffer docbuf) {
    int err = tidyParseBuffer(tdoc, &docbuf);
    if (err >= 0)
        err = parseAux(tdoc);
    return err;
}

int parseFile(TidyDoc tdoc, char *filename) {
    int err = tidyParseFile(tdoc, filename);
    if (err >= 0)
        err = parseAux(tdoc);
    return err;
}
