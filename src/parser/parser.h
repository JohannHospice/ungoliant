#include <tidy.h>

void queryNodeByDoc(TidyNode **buffer, int *size, TidyDoc doc, TidyNode tnod, char *target);
int parseBuffer(TidyDoc tdoc, TidyBuffer docbuf);
int parseFile(TidyDoc tdoc, char *filename);
void queryAttrByNode(char **buffer, TidyNode node, char *target);
int queryAttrByAllNodes(char ***buffers, int nodes_size, TidyNode *nodes, char *target);
void getAllText(char **buffer, TidyDoc doc, TidyNode tnod);
