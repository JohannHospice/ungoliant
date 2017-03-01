#include <curl.h>
#include <tidy.h>

char **queryAttrByAllNodes(TidyNode *nodes, int nodes_size, char *target, char **buffers);
char *queryAttrByNode(TidyNode node, char *target);
int queryNodeByDoc(TidyDoc doc, TidyNode tnod, char *target, TidyNode *res, int size);
int parseCURL(TidyDoc tdoc, TidyBuffer docbuf, CURL *curl);
