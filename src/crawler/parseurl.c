#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "parseurl.h"
#include "../tools.h"

#define REGEX_RELATIVE_URL "^/?([a-zA-Z0-9.-/=&]+)([#\?]?.*)?$"
#define REGEX_ABSOLUTE_URL "^([a-z]{3,9})://([a-zA-Z0-9.@_:\\-]+)/?([a-zA-Z0-9.-/=&]+)([#\?]?.*)?$"

void initEmptyString(char **dst){
    *dst = malloc(sizeof(char));
    *dst[0] = 0;
}

struct url *newUrl(){
    struct url *url = malloc(sizeof(struct url));
    url->protocole = malloc(sizeof(char));
    url->domain = malloc(sizeof(char));
    url->path = malloc(sizeof(char));
    url->other = malloc(sizeof(char));
    url->type = -1;
    return url;
}

void freeUrl(struct url **url){
    free((*url)->protocole);
    free((*url)->domain);
    free((*url)->path);
    free((*url)->other);
    free(*url);
    *url = NULL;
}

int easy_regexec_group(const char *source, const char *patern, regmatch_t *group, size_t group_size) {
    regex_t regex;
    int err = regcomp(&regex, patern, REG_EXTENDED);
    if (err == 0) {
        int match = regexec(&regex, source, group_size, group, 0);
        regfree(&regex);
        return match;
    }
    regfree(&regex);
    printf("Could not compile regular expression.\n");
    return -1;
}

void easy_strcpy_group(char **dst, const char *source, const regmatch_t group) {
    size_t size = group.rm_eo - group.rm_so;
    if(size > 0){
        *dst = calloc(size+1, sizeof(char));
        strncpy(*dst, source + group.rm_so, size);
    }
    else{
        initEmptyString(dst);
    }
}


struct url *parseUrl(const char *source){
    size_t group_size = 5;
    regmatch_t *group = calloc(group_size, sizeof(regmatch_t));

    struct url *url = malloc(sizeof(struct url) +1);

    if(easy_regexec_group(source, REGEX_ABSOLUTE_URL, group, group_size) == 0){
        url->type = 0;
        easy_strcpy_group(&url->protocole, source, group[1]);
        easy_strcpy_group(&url->domain, source, group[2]);
        easy_strcpy_group(&url->path, source, group[3]);
        easy_strcpy_group(&url->other, source, group[4]);

        free(group);
        return url;
    }
    
    initEmptyString(&url->protocole);
    initEmptyString(&url->domain);

    group_size = 3;
    group = realloc(group, group_size * sizeof(regmatch_t));
    if(easy_regexec_group(source, REGEX_RELATIVE_URL, group, group_size) == 0){
        easy_strcpy_group(&url->path, source, group[1]);
        easy_strcpy_group(&url->other, source, group[2]);
        url->type = 1;

        free(group);
        return url;
    }
    url->other = calloc(strlen(source) + 1, sizeof(char));
    strcpy(url->other, source);
    initEmptyString(&url->path);
    url->type = 2;
    
    free(group);
    return url;
}

void printUrl(const struct url *url) {
    printf("type: %d\n", url->type);
    if(url->type == 0) {
        printf("domain: %s\n", url->domain);
        printf("protocole: %s\n", url->protocole);
        printf("path: %s\n", url->path);
        printf("other: %s\n", url->other);
    }
    else if(url->type == 1) {
        printf("path: %s\n", url->path);
        printf("other: %s\n", url->other);
    }
    else if(url->type == 2) {
        printf("other: %s\n", url->other);
    }
}

char *composeUrl(const struct url *url){    
    char *composedUrl = NULL;
    switch(url->type){
        case 0:
            composedUrl = calloc(1 + 4 + strlen(url->protocole) + strlen(url->domain) + strlen(url->path) + strlen(url->other), sizeof(char));
            sprintf(composedUrl, "%s://%s/%s%s", 
                url->protocole, 
                url->domain, 
                url->path, 
                url->other);
            return composedUrl;
        case 1:
            composedUrl = calloc(1 + 1 + strlen(url->path) + strlen(url->other), sizeof(char));
            sprintf(composedUrl, "/%s%s", 
                url->path, 
                url->other);
            return composedUrl;
        case 2:
            composedUrl = calloc(1 + strlen(url->other), sizeof(char));
            strcpy(composedUrl, url->other);
            return composedUrl;
    }
    return composedUrl;
}

void composeUrlBuff(char **buff, const struct url *url){
    if(url->type == 0) {
        *buff = realloc(*buff, sizeof(char) *(1+4 + strlen(url->protocole) + strlen(url->domain) + strlen(url->path) + strlen(url->other)));
        sprintf(*buff, "%s://%s/%s%s", 
            url->protocole, 
            url->domain, 
            url->path, 
            url->other);
    }
    if(url->type == 1) {
        *buff = realloc(*buff, sizeof(char) *(1+1 + strlen(url->path) + strlen(url->other)));
        sprintf(*buff, "/%s%s", url->path, url->other);
    }
    if(url->type == 2) {
        *buff = realloc(*buff, sizeof(char) * (1+strlen(url->other)));
        strcpy(*buff, url->other);
    }
}


int relativeToAbsoluteUrl(struct url *relative, const struct url *base) {
    if(relative->type == 1 || relative->type == 2){
        relative->type = 0;
        relative->domain = realloc(relative->domain, (1+strlen(base->domain))* sizeof(char));
        relative->protocole = realloc(relative->protocole, (1+strlen(base->protocole))* sizeof(char));
        strcpy(relative->domain, base->domain);
        strcpy(relative->protocole, base->protocole);
        return 0;
    }
    return 1;
}