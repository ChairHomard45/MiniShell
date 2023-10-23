#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fonction.h"

#define maxs 101

int main(){
  	char * str;
    int newLen;
    int oldLen;
    /*
    str = strdup("     	chaine de caracteres.   	");
    oldLen = strlen(str);
  	printf("Chaine Avant %d: \"%s\"\n",oldLen, str);
    newLen = trim(str); // str = "chaine de caractères."
    printf("Chaine resultante %d: \"%s\"\n",newLen, str);
    
    printf("\n");

    str = strdup("chaine   	de    	caracteres.");
    printf("Chaine Avant : \"%s\"\n", str);
    clean(str); // str = "chaine de caractères"
  	printf("Chaine resultante : \"%s\"\n", str);
  	
    printf("\n");
    */
   /*
    str = strdup("ls -l>><file_list.txt .log2>");
    oldLen = strlen(str);
    printf("Chaine Avant %d: \"%s\"\n",oldLen, str);
    newLen = separate_s(str, "2;!|&\"<>$ ", maxs);
    printf("Chaine resultante %d: \"%s\"\n",newLen, str);
*/


    //char* str2 = calloc(50, 1);
    str = strdup("$HOME fre<< g2>>fe");
    printf("str2 1= \"%s\"\n", str);
    substenv(str, maxs);
    printf("str2 2= \"%s\"\n", str);
    free(str);
    
    printf("\n");

    char *str3 = strdup("chaine de caracteres a decouper en tokens");
    printf("Chaine:%s\n",str3);
    char* tokens[8];
    int tokenCount = strcut(str3,' ', tokens, 9); // tokens = {"chaine", "de", "caractères", "à", "découper", NULL}
    for (int i = 0; i < tokenCount; i++) {
        printf("Token %d: %s\n", i, tokens[i]);free(tokens[i]);
    }
    free(str3);
}
