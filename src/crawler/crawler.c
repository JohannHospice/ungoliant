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

int isUrlAbsolute(char *url){
  if(url == NULL || url[0] == '\0')
    return -1;
  int absolute = 1, ok = 1;
  const char *req = "http://", *reqs = "https://";
  
  for(int i = 0; url[i] != '\0' && absolute == 1; i++){
    if((i < 7 && url[i] == req[i]) || (i < 8 && url[i] == reqs[i])) { 
    }
    else if(url[i] == '/')
      ok = 0;
    else if(ok == 0)
      absolute = 0;
  }
  if(absolute == 0) printf("%s\n", url);
  
  return absolute;
}

char **normalize(char **attrs, int size, char *url, int url_size) {
  char **value = (char**) calloc(size, sizeof(char*));
  for (int i = 0; i < size; ++i) {
    char *cc = calloc(255, sizeof(char));
    if(isUrlAbsolute(attrs[i]) == 0) {
        int j = 0;
        while(attrs[i][j]!='\0') {
          cc[j] = attrs[i][j];
          j++;
        }
        cc[j+1] = '\0';
        value[i] = cc;
    }
  }
  return value;
}

int size(char *url){
  int i = 0;
  while(url[i]!='\0')
    i++;
  return i;
}

char **crawlUrl(char *url, int rec) {
  TidyBuffer docbuf = {0};
  TidyBuffer tidy_errbuf = {0};
  TidyDoc tdoc = tidyCreate();
  tidyOptSetBool(tdoc, TidyForceOutput, yes);
  tidyOptSetInt(tdoc, TidyWrapLen, 4096);
  tidySetErrorBuffer(tdoc, &tidy_errbuf);
  tidyBufInit(&docbuf);

  char **links;
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

        links = normalize(attrs, nodes_size, url, size(url));

        printf("\nextract %d a[href=]:\n", nodes_size);
        printArrayString(links, nodes_size);

/*
        for (int i = 0; i < nodes_size; ++i)
          crawlUrl(attrs[i], rec - 1);
*/
        free(nodes);
        tidyBufFree(&docbuf);
        tidyBufFree(&tidy_errbuf);
        tidyRelease(tdoc);
      } else
        fprintf(stderr, "%s\n", tidy_errbuf.bp);
    } else {
      perror("error");
      fprintf(stderr, "%s\n", curl_errbuf);
    }
  }
  curl_easy_cleanup(curl);
  return links;
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