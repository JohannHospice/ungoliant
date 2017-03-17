#include <tidy.h>

void queryNodeByDoc(TidyNode **buffer, int *size, TidyDoc doc, TidyNode tnod, char *target);
int parseBuffer(TidyDoc tdoc, TidyBuffer docbuf);
int parseFile(TidyDoc tdoc, char *filename);
void queryAttrByNode(char **buffer, TidyNode node, char *target);
void queryAttrByAllNodes(char ***buffers, TidyNode *nodes, int nodes_size, char *target);