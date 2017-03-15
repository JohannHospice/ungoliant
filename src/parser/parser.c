#include <stdio.h>
#include <curl.h>
#include <tidy.h>
#include <tidybuffio.h>
#include <string.h>
#include "../tools.h"
#define SIZEOF_TIDYNODE sizeof(TidyNode)

void queryNodeByDocRec(TidyNode **buffer, int *size, TidyDoc doc, TidyNode tnod, char *target) {
    TidyNode child;
    for (child = tidyGetChild(tnod); child; child = tidyGetNext(child)) {
        const char *name = tidyNodeGetName(child);
        if (name && strcmp(name, target) == 0) {
            int sizeofBuffer = sizeof(*buffer);
            if (sizeofBuffer < sizeofBuffer + SIZEOF_TIDYNODE)
                *buffer = realloc(*buffer, (*size + 1) * SIZEOF_TIDYNODE);
            (*buffer)[*size] = child;
            (*size)++;
        }
        queryNodeByDocRec(buffer, size, doc, child, target);
    }
}

void queryNodeByDoc(TidyNode **buffer, int *size, TidyDoc doc, TidyNode tnod, char *target) {
    *buffer = malloc(SIZEOF_TIDYNODE * 1);
    *size = 0;
    return queryNodeByDocRec(buffer, size, doc, tnod, target);
}

void queryAttrByNode(char **buffer, TidyNode node, char *target) {
    TidyAttr attr;
    for (attr = tidyAttrFirst(node); attr; attr = tidyAttrNext(attr)) {
        const char *name = tidyAttrName(attr);
        if (name && strcmp(name, target) == 0) {
            char *value = (char*)tidyAttrValue(attr);
            if(value){
                int valueSize = strlen(value);
                *buffer = calloc(valueSize, sizeof(char));
                for (int i = 0; i < valueSize; ++i)
                    (*buffer)[i] = value[i]; 
            }
        }
    }
}

void queryAttrByAllNodes(char ***buffers, TidyNode *nodes, int nodes_size, char *target) {
    *buffers = (char**)calloc(nodes_size, sizeof(char*));
    for (int i = 0; i < nodes_size; ++i)
        queryAttrByNode(&(*buffers)[i], nodes[i], target);
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
