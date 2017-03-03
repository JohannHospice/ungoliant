#include <curl.h>
#include <tidy.h>
#include <tidybuffio.h>
#include "../parser/parser.h"
#include "../tools.h"

uint write_cb(char *in , uint size, uint nmemb, TidyBuffer *out) {
    uint r = size * nmemb;
    tidyBufAppend(out, in , r);
    return r;
}

CURL *initialiseCURL(char *url, char *curl_errbuf, TidyBuffer docbuf) {
    CURL *curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_errbuf);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &docbuf);
        return curl;
    }
    return  NULL;
}

void crawl(char *url) {
    TidyBuffer docbuf = {0};
    TidyBuffer tidy_errbuf = {0};
    TidyDoc tdoc = tidyCreate();
    tidyOptSetBool(tdoc, TidyForceOutput, yes);
    tidyOptSetInt(tdoc, TidyWrapLen, 4096);
    tidySetErrorBuffer(tdoc, &tidy_errbuf);
    tidyBufInit(&docbuf);

    char curl_errbuf[CURL_ERROR_SIZE];
    CURL *curl = initialiseCURL(url, curl_errbuf, docbuf);
    if(curl && !curl_easy_perform(curl)) {
        if (parseCURL(tdoc, docbuf) >= 0) {
            TidyNode *nodesA = malloc(sizeof(TidyNode));
            int nodesA_size = queryNodeByDoc(tdoc, tidyGetRoot(tdoc), "a", nodesA, 0);

            char *attrs[nodesA_size];
            queryAttrByAllNodes(nodesA, nodesA_size, "href", attrs);

            printf("\nlinks:\n");
            printArrayString(attrs, nodesA_size);

            free(nodesA);
            tidyBufFree(&tidy_errbuf);
            tidyBufFree(&docbuf);
            tidyRelease(tdoc);
        }
        else
            fprintf(stderr, "%s\n", tidy_errbuf.bp);
    } else {
        perror("error");
        fprintf(stderr, "%s\n", curl_errbuf);
    }
    curl_easy_cleanup(curl);
}
