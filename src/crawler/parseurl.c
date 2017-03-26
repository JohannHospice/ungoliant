#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "parseurl.h"
#include "../tools.h"

#define REGEX_RELATIVE_URL "^/?([a-zA-Z0-9.-/\?=&#]+)$"
#define REGEX_ABSOLUTE_URL "^([a-z]{3,9})://([a-zA-Z0-9.@_:\\-]+)/?([a-zA-Z0-9.?=&#\\%/\\-]+)?$"


struct url *newUrl(){
    struct url *url = malloc(sizeof(struct url));
    url->protocole = calloc(4, sizeof(char));
    url->domain = calloc(60, sizeof(char));
    url->path = calloc(254, sizeof(char));
    return url;
}

void freeUrl(struct url **url){
    free((*url)->protocole);
    free((*url)->domain);
    free((*url)->path);
    free(*url);
}

struct url *parseUrl(char *source){
    size_t group_size = 4;
    regex_t regex;
    regmatch_t group[group_size];

    if (regcomp(&regex, REGEX_ABSOLUTE_URL, REG_EXTENDED)) {
        printf("Could not compile regular expression.\n");
        return NULL;
    }

    if (regexec(&regex, source, group_size, group, 0) == 0) {
        unsigned int g = 0;
        for (g = 0; g < group_size; g++) {
            if (group[g].rm_so == (size_t)-1)
                break;

            char sourceCopy[strlen(source) + 1];
            strcpy(sourceCopy, source);
            sourceCopy[group[g].rm_eo] = 0;
            printf("Group %u: [%2u-%2u]: %s\n",
            g, group[g].rm_so, group[g].rm_eo,
            sourceCopy + group[g].rm_so);
        }
        regfree(&regex);

        struct url *url=newUrl();
        sscanf(source, "%99[^:]://%99[^/]/%99[^\n]", 
            url->protocole, 
            url->domain, 
            url->path);
        url->type = 0;
        return url;
    }

    if (regcomp(&regex, REGEX_RELATIVE_URL, REG_EXTENDED)) {
        printf("Could not compile regular expression.\n");
        return NULL;
    }

    if (regexec(&regex, source, group_size, group, 0) == 0) {
        unsigned int g = 0;
        for (g = 0; g < group_size; g++) {
            if (group[g].rm_so == (size_t)-1)
                break;

            char sourceCopy[strlen(source) + 1];
            strcpy(sourceCopy, source);
            sourceCopy[group[g].rm_eo] = 0;
            printf("Group %u: [%2u-%2u]: %s\n",
            g, group[g].rm_so, group[g].rm_eo,
            sourceCopy + group[g].rm_so);
        }
        regfree(&regex);

        struct url *url=newUrl();
        //ne pas recupérer 1er / si present
        sscanf(source, "%99[^\n]", url->path);
        url->type = 1;
        return url;
    }
    return NULL;
}

void printUrl(struct url *url){
    printf("protocole: %s\ndomain: %s\npath: %s\ntype: %d\n", 
        url->protocole, 
        url->domain, 
        url->path, 
        url->type);
}

char *composeUrl(struct url *url){
    if(url->type == 0) {
        char *str = calloc(4 + strlen(url->protocole) + strlen(url->domain) + strlen(url->path), sizeof(char));
        sprintf(str, "%s://%s/%s", 
            url->protocole, 
            url->domain, 
            url->path);
        return str;
    }
    if(url->type == 1) {
        char *str = calloc(1 + strlen(url->path), sizeof(char));
        sprintf(str, "/%s", url->path);
        return str;
    }
    return NULL;
}

int relativeToAbsoluteUrl(struct url *relative, struct url *base) {
    if(relative->type == 1){
        relative->domain = malloc(sizeof(base->domain));
        relative->protocole = malloc(sizeof(base->protocole));
        strcpy(relative->domain, base->domain);
        strcpy(relative->protocole, base->protocole);
        relative->type = 0;
        return 0;
    }
    return 1;
}

int isUrlAbsolute(struct url *url) {
    return url->type == 0;
}
int isUrlRelative(struct url *url) {
    return url->type == 1;
}
int isUrlOther(struct url *url) {
    return url->type == 2;
}
