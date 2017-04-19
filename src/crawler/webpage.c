#include <stdio.h>
#include <stdlib.h>
#include "parseurl.h"
#include "webpage.h"
#include "../tools.h"

int getTotalSize(struct treepage *treepage) {
    int sum = 1;
    for (int i = 0; i < treepage->treepage_size; ++i)
        sum += getTotalSize(treepage->treepage[i]);
    return sum;
}
void printInfoPage(struct infopage *infopage) {
    printf("url: %s\ncontent: (\n%s\n)\nlinks_size: %d\nlinks:\n", infopage->url, infopage->content, infopage->links_size);
    printArrayString(infopage->links, infopage->links_size);
}

void printTreepage(struct treepage *treepage) {
    printf("depth: %d\ntreepage_size: %d\n", treepage->depth, treepage->treepage_size);
    printInfoPage(treepage->infopage);
    for (int i = 0; i < treepage->treepage_size; ++i)
        printTreepage(treepage->treepage[i]);
}

void freeInfoPage(struct infopage **infopage) {
    if(*infopage){
        for (int i = 0; i < (*infopage)->links_size; ++i)
            if((*infopage)->links[i])
                free((*infopage)->links[i]);
        if((*infopage)->url)
            free((*infopage)->url);
        if((*infopage)->content)
            free((*infopage)->content);
        free(*infopage);
    }
    *infopage = NULL;
}

void freeTreepage(struct treepage **treepage) {
    for (int i = 0; i < (*treepage)->treepage_size; ++i)
        freeTreepage(&(*treepage)->treepage[i]);
    freeInfoPage(&(*treepage)->infopage);
    free(*treepage);
    *treepage = NULL;
}