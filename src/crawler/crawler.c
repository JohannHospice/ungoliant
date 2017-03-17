#include <curl/curl.h>
#include <tidy.h>
#include <tidybuffio.h>
#include "../parser/parser.h"
#include "crawler.h"
#include "webpage.h"

uint write_cb(char *in, uint size, uint nmemb, TidyBuffer *out) {
    uint r = size * nmemb;
    tidyBufAppend(out, in, r);
    return r;
}

char *dynaclone(char *src){
    int len = strlen(src);
    char *dest = calloc(len, sizeof(char));
    for (int i = 0; i < len; ++i)
        dest[i] = src[i];
    return dest;
}

char *dynaAppend(char *src1, int len1, char *src2, int len2){
    char *dest = calloc(len1 + len2, sizeof(char));
    for (int i = 0; i < len1; ++i)
        dest[i] = src1[i];
    for (int i = len1; i < len1 + len2; ++i)
        dest[i] = src2[i];
    return dest;
}

void append(char **src1, int len1, char *src2, int len2){
    int sizeof_src1 = sizeof(*src1),
        sizeof_src2 = sizeof(src2);

    if(sizeof_src1 < sizeof_src1 + sizeof_src2)
        *src1 = realloc(*src1, sizeof_src1 + sizeof_src2);

    for (int i = len1; i < len1 + len2; ++i)
        *src1[i] = src2[i];

}


void normalizeUrl(char **url, int url_size, char *host, int host_size){
    if(*url[0] == '/'){
        char *tmp = dynaAppend(host, host_size, *url, url_size);
        free(url);
        *url = tmp;
    }
}
/*
regex absolute url
^
((\w{3,9}?):\/\/)?
((?:(\w+):(\w+)@)?([A-Za-z0-9.\-]+\.\w+)(?::(\d+))?)
(([A-Za-z0-9.\/\-%]+)+)?
(?:\?([a-zA-Z0-9_-]+=[a-zA-Z0-9_\-\{\}\[\],;]*|\&)+)?
(?:#[a-zA-Z0-9_-]*)?
$
*/
void normalizeUrls(char **urls, int urls_size, char *host, int host_size){
    for (int i = 0; i < urls_size; ++i) {
        normalizeUrl(&(urls[i]), strlen(urls[i]), host, host_size);
    }
}

/**
 * TODO: save info pages
 */
struct treepage *crawlUrlRec(char *url, int max, int depth) {
    if(depth <= 0)
        return NULL;
    printf(".");
    fflush(stdout);
    struct infopage *tmp = crawlUrl(url);
    
    printf(".");
    fflush(stdout);
    //normalizeUrls(tmp->links, tmp->links_size, tmp->url);
    if(tmp != NULL) {
        printf("1");
        fflush(stdout);
        
        struct treepage *treepage = malloc(sizeof(treepage));
        treepage->infopage = tmp;
        treepage->depth = max - depth;

        treepage->treepage = calloc(treepage->infopage->links_size, sizeof(struct treepage*));
        treepage->treepage_size = 0;
        for (int i = 0; i < treepage->infopage->links_size; ++i){
            struct treepage *tmp = crawlUrlRec(treepage->infopage->links[i], max, depth - 1);
            if(tmp != NULL) {
                treepage->treepage[treepage->treepage_size] = tmp;
                treepage->treepage_size++;
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
            if(curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &rebuilturl) == 0) {
                size_t rebuilturl_len = strlen(rebuilturl);
                infopage->url = calloc( rebuilturl_len, sizeof(char));
                for (int i = 0; i < rebuilturl_len; ++i)
                    infopage->url[i] = rebuilturl[i];
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
            } /*
            else 
                fprintf(stderr, "url: %s\nerror tidy: %s\n", url, tidy_errbuf.bp);
            */
        } 
        /*
        else 
            fprintf(stderr, "url: %s\nerror curl: %s\n", url, curl_errbuf);
        */
        curl_easy_cleanup(curl);
    }
    return infopage;
}