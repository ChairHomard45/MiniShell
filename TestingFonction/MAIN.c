#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fonction.h"
#include <windows.h> //For Windows version

int main(){
  	char * str = strdup("     	chaine de caracteres.   	");
    int oldLen = strlen(str);
  	 printf("Chaine Avant %d: \"%s\"\n",oldLen, str);
    int newLen = trim(str); // str = "chaine de caractères."
    printf("Chaine resultante %d: \"%s\"\n",newLen, str);
    
    printf("\n");

    str = strdup("chaine   	de    	caracteres.");
    printf("Chaine Avant : \"%s\"\n", str);
    clean(str); // str = "chaine de caractères"
  	printf("Chaine resultante : \"%s\"\n", str);
  	
    printf("\n");

    str = strdup(";chaine. de+caracteres;    ");
    oldLen = strlen(str);
    printf("Chaine Avant %d: \"%s\"\n",oldLen, str);
    newLen = separate_s(str, ";.+/?", 28);
    printf("Chaine resultante %d: \"%s\"\n",newLen, str);

    printf("\n");

    char* str2 = calloc(41, 1);
    strcpy(str2, "home = ${HOME} - user = ${USER}");
    substenv(str2, 41);
    printf("str2 = \"%s\"\n", str2);
    free(str2);
    
    printf("\n");

    str = strdup("chaine de caracteres a decouper en tokens");
    char* tokens[8];
    int result = strcut(str,' ', tokens, 8); // tokens = {"chaine", "de", "caractères", "à", "découper", NULL}
    for (int i = 0; i < result+1; i++) {
        printf("tokens[%d] = \"%s\"\n", i+1, tokens[i]);
        free(tokens[i]); // Remember to free the allocated memory
    }

    printf("\n");

    str = strdup("chaine contenant des \"sous chaines\" en 'deux formats'.");
    char* tokens2[8];
    int result2 = strcut_s(str, ' ', tokens2, 8);
    for (int i = 0; i < result2+1; i++) {
        printf("tokens2[%d] = \"%s\"\n", i+1, tokens2[i]);
        free(tokens[i]); // Remember to free the allocated memory
    }
}
