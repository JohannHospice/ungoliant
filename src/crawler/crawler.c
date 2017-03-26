#include <curl/curl.h>
#include <tidy.h>
#include <tidybuffio.h>
#include <string.h>
#include "crawler.h"
#include "parseurl.h"
#include "webpage.h"
#include "../tools.h"
#include "../parser/parser.h"

uint write_cb(char *in, uint size, uint nmemb, TidyBuffer *out) {
    uint r = size * nmemb;
    tidyBufAppend(out, in, r);
    return r;
}

struct treepage *crawlUrlRec(char *url, int max, int depth) {
    if(depth <= 0)
        return NULL;

    printf(".");
    fflush(stdout);

    struct infopage *tmp = crawlUrl(url);

    if(tmp != NULL) {
        printf("1");
        fflush(stdout);

        struct treepage *treepage = malloc(sizeof(treepage));
        treepage->infopage = tmp;
        treepage->depth = max - depth;

        treepage->treepage = calloc(treepage->infopage->links_size, sizeof(struct treepage*));
        treepage->treepage_size = 0;
        for (int i = 0; i < treepage->infopage->links_size; ++i){
            struct url *linkUrl = parseUrl(treepage->infopage->links[i]);

            if(linkUrl->type != -1){
                if(linkUrl->type > 0)
                    relativeToAbsoluteUrl(linkUrl, treepage->infopage->parseurl);
                char *link = composeUrl(linkUrl);
                freeUrl(&linkUrl);
                struct treepage *tmp = crawlUrlRec(link, max, depth - 1);
                if(tmp != NULL) {
                    treepage->treepage[treepage->treepage_size] = tmp;
                    treepage->treepage_size++;
                }
            }
        }
        return treepage;
    }
    printf("0");
    fflush(stdout);
    return NULL;
}

struct infopage *crawlUrl(char *url) {
    struct infopage *infopage = NULL;

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
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(curl, CURLOPT_POSTREDIR, 3);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &docbuf);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_errbuf);
        if (!curl_easy_perform(curl)) {
            char *rebuilturl;
            infopage = malloc(sizeof(struct infopage));
            printf(".");
            fflush(stdout);

            if(curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &rebuilturl) == 0) {
                infopage->url = calloc(strlen(rebuilturl), sizeof(char));
                infopage->parseurl = parseUrl(rebuilturl);
                strcpy(infopage->url, rebuilturl);
                infopage->links_size = 0;
            }

            if (parseBuffer(tdoc, docbuf) >= 0) {
                TidyNode *nodes;
                int nodes_size;
                queryNodeByDoc(&nodes, &nodes_size, tdoc, tidyGetRoot(tdoc), "a");

                queryAttrByAllNodes(&(infopage->links), nodes, nodes_size, "href");
                infopage->links_size = nodes_size;

                free(nodes);
                tidyBufFree(&docbuf);
                tidyBufFree(&tidy_errbuf);
                tidyRelease(tdoc);
            }
        }
        curl_easy_cleanup(curl);
    }
    return infopage;
}