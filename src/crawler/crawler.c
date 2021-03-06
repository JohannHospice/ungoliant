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

int extractAllLinks(char ***array, TidyDoc tdoc){
    TidyNode *nodes;
    int nodes_size;

    queryNodeByDoc(&nodes, &nodes_size, tdoc, tidyGetRoot(tdoc), "a");
    int size = queryAttrByAllNodes(array, nodes_size, nodes, "href");
    
    free(nodes);
    return size;
}

struct treepage *getTreePage(char *url, int max, int depth) {
    if(depth < 1)
        return NULL;

    printf(".");
    fflush(stdout);

    struct infopage *infotmp = getInfoPage(url);
    if(infotmp != NULL) {
        printf("1(%d)", infotmp->links_size);
        fflush(stdout);

        struct treepage *treepage = malloc(sizeof(struct treepage));
        treepage->infopage = infotmp;
        treepage->depth = max - depth;

        treepage->treepage_size = 0;
        treepage->treepage = calloc(infotmp->links_size, sizeof(struct treepage*));
        struct url *baseUrl = parseUrl(infotmp->url);
        for (int i = 0; i < infotmp->links_size; ++i){
            if(infotmp->links[i] == NULL)
                break;
            struct url *linkUrl = parseUrl(infotmp->links[i]);
            if(linkUrl->type != -1){
                if(linkUrl->type > 0)
                    relativeToAbsoluteUrl(linkUrl, baseUrl);
                char *link = composeUrl(linkUrl);
                struct treepage *treetmp = getTreePage(link, max, depth - 1);
                if(treetmp != NULL) {
                    treepage->treepage[treepage->treepage_size] = treetmp;
                    treepage->treepage_size++;
                }
            }
            freeUrl(&linkUrl);
        }
        treepage->treepage = realloc(treepage->treepage, (treepage->treepage_size + 1) * sizeof(struct treepage*));
        return treepage;
    }
    printf("0");
    fflush(stdout);
    return NULL;
}
/**
* 
*/
struct infopage *getInfoPage(char *url) {
    struct infopage *infopage = NULL;

    TidyBuffer docbuf = {0};
    TidyBuffer tidy_errbuf = {0};
    TidyDoc tdoc = tidyCreate();
    tidyOptSetBool(tdoc, TidyForceOutput, yes);
    tidyOptSetInt(tdoc, TidyWrapLen, 4096);
    tidySetErrorBuffer(tdoc, &tidy_errbuf);
    tidyBufInit(&docbuf);

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
        //curl
        if (!curl_easy_perform(curl)) {
            char *rebuilturl;
            infopage = malloc(sizeof(struct infopage));
            infopage->links_size = 0;
            //get url used during curl
            if(curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &rebuilturl) == 0) {
                infopage->url = calloc(strlen(rebuilturl)+1, sizeof(char));
                strcpy(infopage->url, rebuilturl);
            }
            //parse
            if (parseBuffer(tdoc, docbuf) >= 0) {
                infopage->links_size = extractAllLinks(&(infopage->links), tdoc);
                
                /*
                int len = sizeof(docbuf.bp);
                infopage->content = malloc(len);
                memcpy(infopage->content, docbuf.bp, len);
                struct wc *wc;
                int wc_size = 0;
                wordsOccurenceByDoc(&wc, &wc_size, tdoc, tidyGetRoot(tdoc));

                */
                infopage->content = calloc(1,sizeof(char));
                getAllText(&infopage->content, tdoc, tidyGetRoot(tdoc));

                tidyBufFree(&docbuf);
                tidyBufFree(&tidy_errbuf);
            }
        }
        tidyRelease(tdoc);
        curl_easy_cleanup(curl);
    }
    return infopage;
}


struct wc
{
    char *word;
    int count;    
};

struct wc *function(char *sentence){
    struct wc *words = malloc(sizeof(struct wc) * 100);
    /*
    char word[80];

    int j= 0;
    for (int i = 0; i < strlen(sentence); ++i) {
        if(sentence[i] == ' '){
            int added = 1, k = 0;
            while(k < nbWord && added==1) {
                if(strcmp(words[k], word) == 0){
                    words[k].count++;
                    added = 0;
                }
                ++k;
            }
            if(added == 1){
                words = realloc(words, sizeof(struct words) * (nbWord+1));
                words[nbWord].count = 1;
                words[nbWord].word = malloc(sizeof(char) * strlen(tidyWords[i]));
                strcpy(&(words[nbWord].word), tidyWords[i]);
            }
        }else {
            //WARN if j > BOOM
            word[j] += sentence[i]
            j++;
        }
    }
    */
    return words;
}
