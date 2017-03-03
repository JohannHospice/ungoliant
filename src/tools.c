#include <stdio.h>

void printArrayString(char **in , int in_size) {
    for (int i = 0; i < in_size; ++i)
        printf("%s\n", in [i]);
}

void cleanArray(char **buffer, int size){
    for(int i = 0; i < size; i++){
        if(buffer[i] == NULL){
            int j = i;
            while(j< size - 1){
              buffer[j] = buffer[j+1];
            }
            buffer[j+1] = NULL;
        }
    }
}
