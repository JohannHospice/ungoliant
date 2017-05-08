#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "../src/crawler/crawler.h"
#include "../src/crawler/webpage.h"
#include "../src/crawler/parseurl.h"

void testParseurl(){
    char *absStr = "https://github.com/fdsfd/fdgf";
    char *relStr[5] = {"/blabla/fmdls",  "index.html#link", "#blabla", "?s=blabla&k=0", "fdfsfds"};
    printf("\n\tPARSERURL TEST\n\n");

    struct url *relUrl = parseUrl(relStr[1]);
    struct url *absUrl = parseUrl(absStr);

    printf("base: %s\n", relStr[1]);
    printUrl(relUrl);
    printf("\nbase: %s\n", absStr);
    printUrl(absUrl);

    relativeToAbsoluteUrl(relUrl, absUrl);
    
    printf("\nrel to abs\n");
    printUrl(relUrl);
    printf("\ncompose\n");

    char *str = composeUrl(relUrl);
    printf("base: %s\n", str);

    free(str);
    freeUrl(&relUrl);
    freeUrl(&absUrl);
}

void testTreePage(char *url, int depth){
    printf("\n\tCRAWLER TEST on %s with %d of depth\n\n", url, depth);
    printf("relative link not supported\n");
    printf("crawling (1=success, 0=failure, .=crawl/parse)...\n");
    struct treepage *treepage = getTreePage(url, depth, depth);

    if(treepage){
        printf("\nnumber of pages crawled: %d\n\n\tDISPLAY tree\n\n", getTotalSize(treepage));
        printTreepage(treepage);
        freeTreepage(&treepage);
    }
    else
        printf("no result\n");
}

void testInfoPage(char * url){
    printf("\n\tINFOPAGE TEST on %s\n\n", url);
    struct infopage *infopage = getInfoPage(url);
    if(infopage != NULL){
        printInfoPage(infopage);
        freeInfoPage(&infopage);
    }
    else
        printf("no result\n");
}

int main(int argc, char **argv) {
    char *url = argc > 2 ? argv[2] : "https://github.com/";
    int depth = (argc > 3 ? atoi(argv[3]) : 2);
    switch(argc > 1 ? atoi(argv[1]) : 0){
        case 1: 
            testParseurl(url);
        break;
        case 2:
            testInfoPage(url);
        break;
        case 3:
            testTreePage(url, depth);
        break;
        default:
            testParseurl(url);
            testInfoPage(url);
            testTreePage(url, depth);
    }
    return 0;
}
