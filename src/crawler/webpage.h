struct infopage{
    char *url;
    char **links;
    int links_size;
    char *content;
};

struct treepage {
    int depth;
    int treepage_size;
    struct infopage *infopage;
    struct treepage **treepage;
};

int getTotalSize(struct treepage *treepage);
void printInfoPage(struct infopage *infopage);
void printTreepage(struct treepage *treepage);
void freeInfoPage(struct infopage **infopage);
void freeTreepage(struct treepage **treepage);
