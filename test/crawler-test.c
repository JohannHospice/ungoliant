#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "../src/crawler/crawler.h"
#include "../src/crawler/webpage.h"
#include "../src/crawler/parseurl.h"

void testParseurl(){
    /*
    char *relative ="dskqld.html";

    struct url *url1 = parseUrl(sr);
    struct url *url2 = parseUrl(relative);
    printUrl(url1);
    printUrl(url2);
    
    if(isUrlAbsolute(url2) == 0){
        printf("relat\n");
        struct url *url3 = relativeToAbsoluteUrl(relative, url1->domain);
        printUrl(url3);
    }
    */
    char *absStr = "https://github.com/fdsfd/fdgf";
    char *relStr = "/blabla/fmdls";

    struct url *relUrl = parseUrl(relStr);
    printUrl(relUrl);
    
    struct url *absUrl = parseUrl(absStr);
    printUrl(absUrl);

    relativeToAbsoluteUrl(relUrl, absUrl);
    printUrl(relUrl);

    char *str = composeUrl(relUrl);
    printf("%s\n", str);
    freeUrl(&relUrl);
    freeUrl(&absUrl);
    free(str);

}

void testCrawler(int argc, char **argv){
    char *url = (argc > 1) ? argv[1] : "https://github.com/";
    int depth = (argc > 2) ? atoi(argv[2]) : 2;

    printf("\n\tCRAWLER TEST on %s with %d of depth\n\n", url, depth);
    printf("relative link not supported\n");
    printf("crawling (1=success, 0=failure, .=crawl/parse)...\n");
    struct treepage *treepage = crawlUrlRec(url, depth, depth);
    if(treepage){
        printf("\nnumber of pages crawled: %d\n\n\tDISPLAY tree\n\n", getTotalSize(treepage));
        printTreepage(treepage);
        freeTreepage(&treepage);
    }
    else
        printf("no result\n");
}
int main(int argc, char **argv) {
    testParseurl();
    //testCrawler(argc, argv);
    return 0;
}
