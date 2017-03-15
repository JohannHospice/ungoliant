#include <stdio.h>
#include <stdlib.h>

void search(char* c) {
	//L'endroit ou chercher l'index
	strcmp(c, endroitIndex);
	//Resultat strcmp
}

char* marge(char* c) { //peut utiliser la bibl clib?
	char* f;
	int i;
	for(i=0; i<strlen(c);i++) {
		char t;
		t=tolower(c[i]);
		if(t!=NULL){ //caractere speciaux
			if(t=="à" || t=="á" || t=="â" || t=="ã" || t=="ä" || t=="å"")
				t="a";
			else if (t=="è" || t=="é" || t=="ê" || t=="ë")
				t="e";
			else if (t=="ì" || t=="í" || t=="î" || t=="ï")
				t="i";
			else if (t=="ò" || t=="ó" || t=="ô")
				t="o"
			else if (t=="ñ")
				t="n"
			else if (t=="ù" || t=="ú" || t=="û" || t=="ü")
				t="u"
			else if(t=="š")
				t="s";
			else if(t=="ý" || t=="ÿ")
				t="y";
			else if(t=="ç")
				t="c";
		}
		f+=t;
	}
	return f;
}

void searchURL() {
	search(char* mot);
	//Chercher liste URL correspondant au mot
}

void returnURL() {
	// return searchURL
}