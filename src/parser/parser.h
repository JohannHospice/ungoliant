#include <tidy.h>
#include <tidybuffio.h>
#include <curl.h>

CURL *download(char *url);
int parser(CURL *curl);