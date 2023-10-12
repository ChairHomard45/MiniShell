#include "parser.h"

int main(){
  	 str = strdup("     	chaine de caractères.   	");
  	 printf("Chaîne Avant : \"%s\"\n", str);
    trim(str); // str = "chaine de caractères."
    printf("Chaîne résultante : \"%s\"\n", str);
    
    str = strdup("chaine   	de    	caractères.");
    printf("Chaîne Avant : \"%s\"\n", str);
    clean(str); // str = "chaine de caractères"
  	 printf("Chaîne résultante : \"%s\"\n", str);
  	 
  	 
}
