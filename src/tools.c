#include <stdio.h>

void printArrayString(char **arr , int arr_size) {
    int i = 0;
    for (i = 0; i < arr_size; ++i)
        printf("%s\n", arr[i]);
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
