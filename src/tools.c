#include <stdio.h>

void printArrayString(char **in , int in_size) {
    for (int i = 0; i < in_size; ++i)
        printf("%s\n", in [i]);
}
