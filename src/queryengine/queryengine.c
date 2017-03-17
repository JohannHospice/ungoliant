#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*void search(char* c) {
	//L'endroit ou chercher l'index
	strcmp(c, endroitIndex);
	//Resultat strcmp
}*/

char* marge(char* c) { //peut utiliser la bibl clib?
	char* f=malloc(100*sizeof(char));
	int i;
	for(i=0; c[i]!='\0';i++) {
		c[i]=tolower(c[i]);
		//printf("%c\n",c[i]);
		if(c[i]){ //caractere speciaux
			printf("%d\n", c[i]);
			if(c[i]=='à' || c[i]=='á' || c[i]=='â' || c[i]=='ã' || c[i]=='ä' || c[i]=='å')
				c[i]='a';
			else if (c[i]=='è' || c[i]=='é' || c[i]=='ê' || c[i]=='ë')
				c[i]='e';
			else if (c[i]=='ì' || c[i]=='í' || c[i]=='î' || c[i]=='ï')
				c[i]='i';
			else if (c[i]=='ò' || c[i]=='ó' || c[i]=='ô')
				c[i]='o';
			else if (c[i]=='ñ')
				c[i]='n';
			else if (c[i]=='ù' || c[i]=='ú' || c[i]=='û' || c[i]=='ü')
				c[i]='u';
			else if(c[i]=='š')
				c[i]='s';
			else if(c[i]=='ý' || c[i]=='ÿ')
				c[i]='y';
			else if(c[i]=='ç')
				c[i]='c';
		}
		f+=c[i];
		printf("%s\n", f);
	}
	return f;
	free(f);
}

/*void searchURL() {
	search(char* mot);
	//Chercher liste URL correspondant au mot
}

void returnURL() {
	// return searchURL
}*/

int main() {
	char* test = malloc(100*sizeof(char));
	//int i;
	scanf("%s", test);
	printf("%s\n", test);
	/*for(i=0; test[i]!='\0'; i++) {
		test[i] = tolower(test[i]);
		printf("%c\n",test[i]);
	}*/
	char* r = marge(test);
	printf("%s\n", r);
	free(test);
	return 0;
}

