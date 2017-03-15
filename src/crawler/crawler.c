#include <curl.h>
#include <tidy.h>
#include <string.h>
#include <tidybuffio.h>
#include "crawler.h"
#include "../tools.h"
#include "../parser/parser.h"

uint write_cb(char *in, uint size, uint nmemb, TidyBuffer *out) {
    uint r = size * nmemb;
    tidyBufAppend(out, in, r);
    return r;
}

void crawlUrlRec(char *url, int rec) {
    if(rec >= 0) {
        printf("rec %d\n", rec);
        struct info_page *info_page = crawlUrl(url);
        if(info_page) {
            info_page->info_pages = calloc(info_page->links_size, sizeof(struct info_page*));
            printInfoPage(info_page);
            for (int i = 0; i < info_page->links_size; ++i)
                crawlUrlRec(info_page->links[i], rec - 1);
            freeInfoPage(&info_page);
        }
		printf("\n");
    }
}

struct info_page *crawlUrl(char *url) {
    struct info_page *info_page = NULL;

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
            char *tmp;
            info_page = malloc(sizeof(struct info_page));
            if(curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &tmp) == 0) {
                int tmplen = strlen(tmp);
                info_page->url = calloc(tmplen, sizeof(char));
                for (int i = 0; i < tmplen; ++i)
                    info_page->url[i] = tmp[i];
            }
            if (parseBuffer(tdoc, docbuf) >= 0) {
                TidyNode *nodes;
                int nodes_size;
                queryNodeByDoc(&nodes, &nodes_size, tdoc, tidyGetRoot(tdoc), "a");

                queryAttrByAllNodes(&(info_page->links), nodes, nodes_size, "href");
                info_page->links_size = nodes_size;

                free(nodes);
                tidyBufFree(&docbuf);
                tidyBufFree(&tidy_errbuf);
                tidyRelease(tdoc);
            } else {
                fprintf(stderr, "url: %s\nerror tidy: %s\n", url, tidy_errbuf.bp);
                info_page = NULL;
            }
        } else {
            fprintf(stderr, "url: %s\nerror curl: %s\n", url, curl_errbuf);
            info_page = NULL;
        }
        curl_easy_cleanup(curl);
    }
    return info_page;
}

void printInfoPage(struct info_page *info_page) {
    printf("url: %s\nlinks_size: %d\nlinks:\n", info_page->url, info_page->links_size);
    printArrayString(info_page->links, info_page->links_size);
}

void freeInfoPage(struct info_page **info_page) {
    for (int i = 0; i < (*info_page)->links_size; ++i)
        free((*info_page)->links[i]);
    free((*info_page)->url);
    free(*info_page);
    *info_page = NULL;
}