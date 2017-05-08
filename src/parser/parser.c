#include <stdio.h>
#include <tidy.h>
#include <tidybuffio.h>
#define SIZEOF_TIDYNODE sizeof(TidyNode)

void getAllText(char **buffer, TidyDoc doc, TidyNode tnod) {
    TidyNode child;
    for (child = tidyGetChild(tnod); child; child = tidyGetNext(child)) {        
        TidyNodeType nt = tidyNodeGetType(child);
        TidyTagId nid = tidyNodeGetId(child);
        ctmbstr name = tidyNodeGetName(child);
        if (!name && nt != TidyNode_Comment && 
                nid != TidyTag_SCRIPT && nid != TidyTag_STYLE){
            TidyBuffer buf;
            tidyBufInit(&buf);
            tidyNodeGetText(doc, child, &buf);
            if (buf.bp && buf.size) {
                char *cp = (char *) buf.bp;
                int len = strlen(cp);
                if (len) {
                    int sizeofNeeded = (strlen(*buffer) + len + 1) * sizeof(char);
                    if (sizeof(*buffer) < sizeofNeeded)
                        *buffer = realloc(*buffer, sizeofNeeded);
                    strcat(*buffer, cp);
                }
            }
            tidyBufFree(&buf);
        }
        getAllText(buffer, doc, child);
    }
}

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
    queryNodeByDocRec(buffer, size, doc, tnod, target);
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
int queryAttrByAllNodes(char ***buffers, int nodes_size, TidyNode *nodes, char *target) {
    *buffers = (char**)calloc(nodes_size + 1, sizeof(char*));
    int j = 0;
    for (int i = 0; i < nodes_size; ++i){
        if(queryAttrByNode(&(*buffers)[j], nodes[i], target) == 0)
            j++;
    }
    *buffers = (char**)realloc(*buffers, sizeof(char*) * (j));
    return j;
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
