/*
  Projet minishell - Licence 3 Info - PSI 2023
 
  Nom :
  Prénom :
  Num. étudiant :
  Groupe de projet :
  Date :
 
  Parsing de la ligne de commandes utilisateur (implémentation).
 
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
//#include <ctype.h>//Pour la version avec isspace



int trim(char* str) {
      if (str == NULL) {
    	return -1; // Code d'erreur pour un parametre invalide
	}
	if (strlen(str) == 0){
		return 0;
	}

	int start = 0;
	

	while (str[start] == ' '||str[start] == '\t'||str[start] == '\n'){start++;} //Version sans isspace
	//while (isspace(str[start])){start++;} //Version avec isspace
	if(start){
		char * tmp = strdup(str);
    	strcpy(str, (char *) (tmp+start)); //on supprime les espaces
    	free(tmp);
	}
	
	int end = strlen(str) - 1;
	while(str[end] == ' '||str[end] == '\t'||str[end] == '\n'){str[end]='\0';end--;} //Sans Isspace
	//while (isspace(str[end])){str[end]='\0';end--;} //Avec isspace
	return strlen(str);
}

int clean(char* str) {
if (str == NULL) {
        return -1; // Gérer le cas où la chaîne est nulle
    }
	if (strlen(str) == 0){
		return 0;
	}

	int Ecriture = 0;
	int Lecture = 0;
	bool LastCharacter = false;

	while(Lecture < strlen(str)){
		if (!(str[Lecture]==' ' || str[Lecture] == '\t' || str[Lecture] == '\n')){
			str[Ecriture]=str[Lecture];
			if (str[Lecture] == ' ') {
                LastCharacter = true;
            } else {
                LastCharacter = false;
            }
			Ecriture++;
		} else if (str[Lecture] == ' ' && !LastCharacter) {
            str[Ecriture] = str[Lecture];
            LastCharacter = true;
            Ecriture++;
        }
		Lecture++;
	}
	str[Ecriture] = '\0';
	return strlen(str);
}

int separate_s(char* str, const char* s, size_t max) {
    if (str == NULL || s == NULL || max == 0) {
        return -1; // Indicate an error due to invalid inputs
    }

    size_t newLen = 0;
    int idx_str = 0;

    // Create a temporary buffer
    char* tmp = (char*)malloc(max);

    if (tmp == NULL) {
        return -1; // Allocation error
    }

    while (str[idx_str] != '\0' && newLen < max - 1) {
        char character = str[idx_str];
	printf("tmp [idx_str]:%s\n",tmp);
        if (strchr(s, character) != NULL) {
            // Handle special characters
            switch (character) {
            	case ';':
            		tmp[newLen++] = ' ';
                        tmp[newLen++] = character;
                        tmp[newLen++] = ' ';
            		break;
                case '&':
                    if (str[idx_str + 1] == '&') {
                        // Handle '&&' case
                        tmp[newLen++] = ' '; // Insert space before '&&'
                        tmp[newLen++] = character;
                        tmp[newLen++] = character;
                        tmp[newLen++] = ' '; // Insert space after '&&'
                        idx_str++; // Skip one character for '&'
                    } else if (str[idx_str + 1] == '>') {
                        // Handle '&>' case
                        tmp[newLen++] = ' '; // Insert space before '&>'
                        tmp[newLen++] = character;
                        tmp[newLen++] = ' '; // Insert space after '&>'
                        idx_str++; // Skip one character for '>'
                    } else {
                        // Handle '&' case
                        tmp[newLen++] = ' '; // Insert space before '&'
                        tmp[newLen++] = character;
                        tmp[newLen++] = ' '; // Insert space after '&'
                    }
                    break;
                // Handle other cases similarly with spaces before and after
                case '|':
                    if (str[idx_str + 1] == '|') {
                        // Handle '||' case
                        tmp[newLen++] = ' ';
                        tmp[newLen++] = character;
                        tmp[newLen++] = character;
                        tmp[newLen++] = ' ';
                        idx_str++; // Skip one character for '|'
                    } else {
                        // Handle '|' case
                        tmp[newLen++] = ' ';
                        tmp[newLen++] = character;
                        tmp[newLen++] = ' ';
                    }
                    break;
                case '<':
                    // Handle '<' case
                    tmp[newLen++] = ' ';
                    tmp[newLen++] = character;
                    tmp[newLen++] = ' ';
                    break;
                case '>':
                    if (str[idx_str + 1] == '>') {
                        // Handle '>>' case
                        tmp[newLen++] = ' ';
                        tmp[newLen++] = character;
                        tmp[newLen++] = character;
                        tmp[newLen++] = ' ';
                        idx_str++; // Skip one character for '>'
                    } else {
                        // Handle '>' case
                        tmp[newLen++] = ' ';
                        tmp[newLen++] = character;
                        tmp[newLen++] = ' ';
                    }
                    break;
                case '2':
                    if (str[idx_str + 1] == '>' && str[idx_str + 2] == '>') {
                        // Handle '2>>' case
                        tmp[newLen++] = ' ';
                        tmp[newLen++] = character;
                        tmp[newLen++] = character;
                        tmp[newLen++] = str[idx_str + 2];
                        tmp[newLen++] = str[idx_str + 3];
                        tmp[newLen++] = ' ';
                        idx_str += 2; // Skip two characters for '>>'
                    } else if (str[idx_str + 1] == '>') {
                        // Handle '2>' case
                        tmp[newLen++] = ' ';
                        tmp[newLen++] = character;
                        tmp[newLen++] = str[idx_str + 1];
                        tmp[newLen++] = ' ';
                        idx_str++; // Skip one character for '>'
                    }
                    break;
                default:
                    // Do nothing
                    break;
            }
        }else{// Copy non-special character to temporary buffer
            tmp[newLen] = character;
            newLen++;
        }

        idx_str++;
    }
    
    printf("tmp = %s newLen = %ld max = %ld\n",tmp,newLen,max);
    
    // Null-terminate the temporary buffer
    tmp[newLen] = '\0';

    // Copy the modified string back to the original string
    strcpy(str,tmp);
    if(str == NULL){
    	printf("Error Copy");
    	return -1;
    }

    // Free the temporary buffer
    free(tmp);

    return 0; // Success
}

int substenv(char* str, size_t max) {
    if (str == NULL || max <= 0) {
        printf("Invalid parameters\n");
        return -1; // Invalid parameters
    }

    size_t strLen = strlen(str);

    for (size_t i = 0; i < strLen; ++i) {
        if (str[i] == '$' && i + 1 < strLen) {
            if (str[i + 1] == '{') {
                size_t start = i + 2;
                size_t end = start;

                while (end < strLen && str[end] != '}') {
                    ++end;
                }

                if (end < strLen) {
                    char varName[64];
                    strncpy(varName, str + start, end - start);
                    varName[end - start] = '\0';

                    char* envValue = getenv(varName);

                    if (envValue != NULL) {
                        size_t envValueLen = strlen(envValue);
                        size_t replaceLen = end - i + 1;

                        if (envValueLen <= max - strLen + replaceLen) {
                            memmove(str + i + envValueLen, str + end + 1, strLen - end);
                            memcpy(str + i, envValue, envValueLen);
                            strLen = strLen - replaceLen + envValueLen;
                            i += envValueLen - 1;
                        } else {
                            printf("Insufficient space to substitute the variable\n");
                            return -1; // Insufficient space
                        }
                    } else {
                        printf("Environment variable '%s' not found\n", varName);
                        // Handle the case where the environment variable is not found as needed
                    }
                }
            } else {
                size_t start = i + 1;
                size_t end = start;

                while (end < strLen && (isalnum(str[end]) || str[end] == '_')) {
                    ++end;
                }

                char varName[64];
                strncpy(varName, str + start, end - start);
                varName[end - start] = '\0';

                char* envValue = getenv(varName);

                if (envValue != NULL) {
                    size_t envValueLen = strlen(envValue);
                    size_t replaceLen = end - i;

                    if (envValueLen <= max - strLen + replaceLen) {
                        memmove(str + i + envValueLen, str + end, strLen - end);
                        memcpy(str + i, envValue, envValueLen);
                        strLen = strLen - replaceLen + envValueLen;
                        i += envValueLen - 1;
                    } else {
                        printf("Insufficient space to substitute the variable\n");
                        return -1; // Insufficient space
                    }
                } else {
                    printf("Environment variable '%s' not found\n", varName);
                    // Handle the case where the environment variable is not found as needed
                }
            }
        }
    }

    str[strLen] = '\0';
    return strLen;
}


int strcut(char* str, char sep, char** tokens, size_t max) {
    int tokenCount = 0;
    char* token = strtok(str, &sep);

    while (token != NULL && tokenCount < max) {
        tokens[tokenCount] = token;
        token = strtok(NULL, &sep);
        tokenCount++;
    }
    return tokenCount;
}

// Optionnel
int strcut_s(char* str, char sep, char** tokens, size_t max) {
    if (str == NULL || tokens == NULL || max == 0) {
        return -1; // Error: Invalid parameters
    }
    return 0;
}
