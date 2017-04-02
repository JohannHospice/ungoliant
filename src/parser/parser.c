#include <stdio.h>
#include <tidy.h>
#include <tidybuffio.h>
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
    *buffer = malloc(SIZEOF_TIDYNODE);
    *size = 0;
    return queryNodeByDocRec(buffer, size, doc, tnod, target);
}

int queryAttrByNode(char **buffer, TidyNode node, char *target) {
    TidyAttr attr;
    for (attr = tidyAttrFirst(node); attr; attr = tidyAttrNext(attr)) {
        const char *name = tidyAttrName(attr);
        if (name && strcmp(name, target) == 0) {
            char *value = (char*)tidyAttrValue(attr);
            if(value != NULL){
                *buffer = calloc(strlen(value)+1, sizeof(char));
                strcpy(*buffer, value);
                return 0;
            }
        }
    }
    return 1;
}
//WARN
void queryAttrByAllNodes(char ***buffers, TidyNode *nodes, int nodes_size, char *target) {
    *buffers = (char**)calloc(nodes_size + 1, sizeof(char*));
    int j = 0;
    for (int i = 0; i < nodes_size; ++i){
        if(queryAttrByNode(&(*buffers)[i-j], nodes[i], target) == 0){
            j++;
            *buffers = realloc(*buffers, sizeof(char*) * (nodes_size - j + 1));
        }
    }
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
