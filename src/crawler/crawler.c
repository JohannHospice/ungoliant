#include <curl.h> 
#include <tidy.h> 
#include <tidybuffio.h> 
#include "../parser/parser.h"
#include "../tools.h"

uint write_cb(char *in, uint size, uint nmemb, TidyBuffer *out) {
  uint r = size * nmemb;
  tidyBufAppend(out, in, r);
  return r;
}

void crawlUrl(char *url) {
  TidyBuffer docbuf = {0};
  TidyBuffer tidy_errbuf = {0};
  TidyDoc tdoc = tidyCreate();
  tidyOptSetBool(tdoc, TidyForceOutput, yes);
  tidyOptSetInt(tdoc, TidyWrapLen, 4096);
  tidySetErrorBuffer(tdoc, &tidy_errbuf);
  tidyBufInit( &docbuf);

  char curl_errbuf[CURL_ERROR_SIZE];
  CURL *curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_errbuf);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &docbuf);
    if (!curl_easy_perform(curl)) {
      if (parseBuffer(tdoc, docbuf) >= 0) {
        TidyNode *nodes = malloc(sizeof(TidyNode) *100);
        int nodes_size = queryNodeByDoc(nodes, 0, tdoc, tidyGetRoot(tdoc), "a");

        char *attrs[nodes_size];
        queryAttrByAllNodes(attrs, nodes, nodes_size, "href");

        printf("\nextract %d a[href=]:\n", nodes_size);
        printArrayString(attrs, nodes_size);

        free(nodes);
        tidyBufFree( &docbuf);
        tidyBufFree( &tidy_errbuf);
        tidyRelease(tdoc);
      } else
        fprintf(stderr, "%s\n", tidy_errbuf.bp);
    } else {
      perror("error");
      fprintf(stderr, "%s\n", curl_errbuf);
    }
  }
  printf("# end\n");
  curl_easy_cleanup(curl);
}

//ne fonctionne pas encore
void crawlFile(char *filename) {
  TidyBuffer docbuf = {0};
  TidyBuffer tidy_errbuf = {0};
  TidyDoc tdoc = tidyCreate();
  tidyOptSetBool(tdoc, TidyForceOutput, yes);
  tidyOptSetInt(tdoc, TidyWrapLen, 4096);
  tidySetErrorBuffer(tdoc, &tidy_errbuf);
  tidyBufInit( &docbuf);

  if (parseFile(tdoc, filename) >= 0) {
    TidyNode *nodes = malloc(sizeof(TidyNode) *100);
    int nodes_size = queryNodeByDoc(nodes, 0, tdoc, tidyGetRoot(tdoc), "a");

    char *attrs[nodes_size];
    queryAttrByAllNodes(attrs, nodes, nodes_size, "href");

    printf("\nextract %d a[href=]:\n", nodes_size);
    printArrayString(attrs, nodes_size);

    free(nodes);
    tidyBufFree( &docbuf);
    tidyBufFree( &tidy_errbuf);
    tidyRelease(tdoc);
  } else
    fprintf(stderr, "%s\n", tidy_errbuf.bp);
  printf("# end\n");
}