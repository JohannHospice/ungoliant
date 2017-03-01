#include "../src/parser/parser.h"
#include "../src/parser/tools.h"

void printArrayString(char **in , int in_size) {
    for (int i = 0; i < in_size; ++i)
        printf("%s\n", in [i]);
}

void test(char *url) {
    TidyBuffer docbuf = {0};
    TidyBuffer tidy_errbuf = {0};
    TidyDoc tdoc = tidyCreate();
    tidyOptSetBool(tdoc, TidyForceOutput, yes);
    tidyOptSetInt(tdoc, TidyWrapLen, 4096);
    tidySetErrorBuffer(tdoc, &tidy_errbuf);
    tidyBufInit(&docbuf);

    char curl_errbuf[CURL_ERROR_SIZE];
    CURL *curl = initialiseCURL(url, curl_errbuf, docbuf);
    //fprintf(stderr, "%s\n", tidy_errbuf.bp);
    if (curl && parseCURL(tdoc, docbuf, curl) >= 0) {
        TidyNode *nodesA = malloc(sizeof(TidyNode));
        int nodesA_size = queryNodeByDoc(tdoc, tidyGetRoot(tdoc), "a", nodesA, 0);

        char *attrs[nodesA_size];
        queryAttrByAllNodes(nodesA, nodesA_size, "href", attrs);

        printf("\nlinks:\n");
        printArrayString(attrs, nodesA_size);

        free(nodesA);
        curl_easy_cleanup(curl);
    } else{
        perror("error");
        fprintf(stderr, "%s\n", curl_errbuf);
    }

    tidyBufFree(&docbuf);
    tidyBufFree(&tidy_errbuf);
    tidyRelease(tdoc);
}

int main(int argc, char **argv) {
    test("https://curl.haxx.se/libcurl/");
    return 0;
}