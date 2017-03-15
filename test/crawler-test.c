#include "../src/crawler/crawler.h"
#include <stdio.h>
int main(int argc, char **argv) {
	printf("\n\tCRAWLER TEST on %s\n\n", argv[1]);
    if(argc < 2){
    	printf("Need argument url\n");
	    return 0;
    }
	crawlUrlRec(argv[1], 2);
    return 0;
}
