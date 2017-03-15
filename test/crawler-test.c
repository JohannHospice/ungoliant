#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/crawler/crawler.h"
#include "../src/crawler/webpage.h"

int main(int argc, char **argv) {
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
    return 0;
}
