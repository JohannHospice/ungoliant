//#include "../tools.h"
struct info_page{
	char *url;
	char **links;
	int links_size;
	struct info_page **info_pages;
};

void crawlUrlRec(char *url, int rec);
void printInfoPage(struct info_page *info_page);
struct info_page *crawlUrl(char *url);
void crawlFile(char *filename);
void freeInfoPage(struct info_page **info_page);