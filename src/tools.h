struct string{
	char *value;
};

void printArrayString(char **arr , int arr_size);
char *dynaclone(char *src);
char *dynaAppend(char *src1, int len1, char *src2, int len2);
void append(char **src1, int len1, char *src2, int len2);
