#include <stdio.h>
#include <curl.h>
#include <tidy.h>
#include <tidybuffio.h>
#include <string.h>
#include "../tools.h"
#define SIZEOF_TIDYNODE sizeof(TidyNode)

int queryNodeByDoc(TidyDoc doc, TidyNode tnod, char *target, TidyNode *buffer, int size) {
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
        size = queryNodeByDoc(doc, child, target, buffer, size);
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
    for (int i = 0; i < nodes_size; ++i)
        buffers[i] = queryAttrByNode(nodes[i], target);
    return buffers;
}

void cleanArray(char **buffer, int size){
    for(int i = 0; i < size; i++){
        if(buffer[i] == NULL){
            int j = i;
            while(j< size - 1){
              buffer[j] = buffer[j+1];
            }
            buffer[j+1] = NULL;
        }
    }
}

int parseCURL(TidyDoc tdoc, TidyBuffer docbuf) {
    int err = tidyParseBuffer(tdoc, &docbuf);
    if (err >= 0)
        err = tidyCleanAndRepair(tdoc);
    if (err >= 0)
        err = tidyRunDiagnostics(tdoc);
    return err;
}
