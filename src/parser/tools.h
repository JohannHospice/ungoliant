#include <tidybuffio.h>
#include <curl.h>

uint write_cb(char *in, uint size, uint nmemb, TidyBuffer *out);
CURL *initialiseCURL(char *url, char *curl_errbuf, TidyBuffer docbuf);