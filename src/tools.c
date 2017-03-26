#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *dynaclone(char *src){
    int len = strlen(src);
    char *dest = calloc(len, sizeof(char));
    for (int i = 0; i < len; ++i)
        dest[i] = src[i];
    return dest;
}

char *dynaAppend(char *src1, int len1, char *src2, int len2){
    char *dest = calloc(len1 + len2, sizeof(char));
    for (int i = 0; i < len1; ++i)
        dest[i] = src1[i];
    for (int i = len1; i < len1 + len2; ++i)
        dest[i] = src2[i];
    return dest;
}

void append(char **src1, int len1, char *src2, int len2){
    int sizeof_src1 = sizeof(*src1),
        sizeof_src2 = sizeof(src2);

    if(sizeof_src1 < sizeof_src1 + sizeof_src2)
        *src1 = realloc(*src1, sizeof_src1 + sizeof_src2);

    for (int i = len1; i < len1 + len2; ++i)
        *src1[i] = src2[i];

}

void printArrayString(char **arr , int arr_size) {
    for (int i = 0; i < arr_size; ++i)
        printf("\t- %s\n", arr[i]);
}

void cleanArray(char **buffer, int size){
    int i = 0;
    for (i = 0; i < size; i++){
        if(buffer[i] == NULL){
            int j = i;
            while(j< size - 1){
              buffer[j] = buffer[j+1];
            }
            buffer[j+1] = NULL;
        }
    }
}
