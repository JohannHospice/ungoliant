#include <curl.h>
#include <tidy.h>

int queryNodeByDoc(TidyNode *buffer, int size, TidyDoc doc, TidyNode tnod, char *target);
char *queryAttrByNode(TidyNode node, char *target);
char **queryAttrByAllNodes(char **buffers, TidyNode *nodes, int nodes_size, char *target);
int parseBuffer(TidyDoc tdoc, TidyBuffer docbuf);
int parseFile(TidyDoc tdoc, char *filename);