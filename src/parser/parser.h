#include <tidy.h>
#include <tidybuffio.h>
#include <curl.h>

void dumpNode(TidyDoc doc, TidyNode tnod, int indent);
CURL *download(char *url);
int parser(CURL *curl);