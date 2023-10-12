#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fonction.h"

int main(){
  	char * str = strdup("     	chaine de caracteres.   	");
  	 printf("Chaine Avant : \"%s\"\n", str);
    trim(str); // str = "chaine de caractères."
    printf("Chaine resultante : \"%s\"\n", str);
    
    str = strdup("chaine   	de    	caracteres.");
    printf("Chaine Avant : \"%s\"\n", str);
    clean(str); // str = "chaine de caractères"
  	 printf("Chaine resultante : \"%s\"\n", str);
  	 
}
