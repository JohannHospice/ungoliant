#include "../src/crawler/crawler.h"
#include <stdio.h>
int main(int argc, char **argv) {
    if(argc < 2)
    	printf("Need argument url\n");
    else
    	crawlUrl(argv[1], 3);
    return 0;
}
