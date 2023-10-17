#include <stddef.h>

//#include <ctype.h>//Pour la version avec isspace


typedef enum {false, true} bool;

int trim(char *str){
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
int separate_s(char* str, char* s, size_t max) {
    if (str == NULL || s == NULL || max <= 0) {
        return -1; // Erreur : parametres invalides
    }

	char* out = (char *) calloc(max, sizeof(char));
	int Ecriture = 0;
	int Lecture = 0;
	int capacite = 0;
	char Character;

	while(str[Lecture] != '\0' || strlen(out) >= max - 1){
		Character = str[Lecture];
		if(strchr(s,Character)){
			if(Lecture!=0){if(str[Lecture-1]!=' ') strcat(out," ");} //ne met pas d'espace avant la chaine
			strncat(out,str+Lecture,1);
			if (str[Lecture+1] != ' ' && str[Lecture+1] != '\0') strcat(out, " ");
		}else{
			strncat(out, str+Lecture, 1);
		}
		if(strlen(out) >= max){
			break;
		}
		Lecture++;
	}
	
	strcpy(str,out);
	free(out);
    return strlen(str);
}
int substenv(char* str, size_t max) {
    if (str == NULL || max <= 0) {
        return -1; // Erreur : paramètres invalides
    }
    
    char* out = (char*)calloc(max, sizeof(char));
    int Ecriture = 0;
    int Lecture = 0;

    while (str[Lecture] != '\0' && Ecriture < max - 1) {
        if (str[Lecture] == '$') {
            // If it's a '$'
            if (str[Lecture + 1] == '{') {
                // Variable of the form ${VAR}
                Lecture += 2; // Skip the "{"
                size_t varStart = Lecture;
                while (str[Lecture] != '}' && str[Lecture] != '\0') {
                    Lecture++;
                }
                if (str[Lecture] == '}') {
                    // Variable found
                    str[Lecture] = '\0'; // Temporarily terminate the string
                    char* envVar = str + varStart;
                    char* envValue = getenv(envVar);
                    if (envValue != NULL) {
                        size_t envLen = strlen(envValue);
                        if (Ecriture + envLen < max) {
							strncpy(out + Ecriture, envValue, envLen);
                            Ecriture += envLen;
							if(str[Lecture]==' ')out[Ecriture++] = ' ';
                        }
                    }
                    // Resume the original string after the variable
                    str[Lecture] = '}';
					Lecture++;
                }
            } else {
                // Variable of the form $VAR
                Lecture++;
                size_t varStart = Lecture;
                while (str[Lecture] != ' ' && str[Lecture] != '\0') {
                    Lecture++;
                }
                // Copy the variable name
                size_t varLen = Lecture - varStart;
                if (varLen > 0) {
                    str[Lecture] = '\0'; // Temporarily terminate the string
                    char* envVar = str + varStart;
                    char* envValue = getenv(envVar);
                    if (envValue != NULL) {
                        size_t envLen = strlen(envValue);
                        if (Ecriture + envLen < max) {
                            strncpy(out + Ecriture, envValue, envLen);
                            Ecriture += envLen;
							if(Ecriture + 1 < max)out[Ecriture++] = ' ';
                        }
                    }
                    // Resume the original string after the variable
                    str[Lecture] = (str[Lecture] == ' ') ? ' ' : '\0';
					Lecture++;
                }
            }
        } else {
            // Copy normal characters
            out[Ecriture++] = str[Lecture++];
        }
    }

    // Terminate the resulting string
    out[Ecriture] = '\0';

    // Copy the resulting string back to the original string (if it fits)
    if (Ecriture < max) {
        strcpy(str, out);
    }else {
        // If the resulting string is too long, truncate it
        str[max - 1] = '\0';
    }

    free(out);

    return 0;
}
int strcut(char* str, char sep, char** tokens, size_t max) {
    if (str == NULL || tokens == NULL || max == 0) {
        return -1; // Error: Invalid parameters
    }

    size_t token_count = 0;
    char* token_start = str;
    size_t str_len = strlen(str);

    for (size_t i = 0; i <= str_len; i++) {
        if (str[i] == sep || str[i] == '\0') {
            // Separator found or the end of the string; create a token
            size_t token_length = i - (token_start - str);
            tokens[token_count] = (char*)malloc(token_length + 1);
            if (tokens[token_count] == NULL) {
                // Handle memory allocation failure
                return -1;
            }
            strncpy(tokens[token_count], token_start, token_length);
            tokens[token_count][token_length] = '\0';
            token_count++;
            token_start = str + i + 1; // Move to the next character after the separator

            if (token_count == max - 1) {
                // If we have reached the max - 1 tokens, create the last one as NULL and return
                tokens[token_count] = NULL;
                return token_count;
            }
        }
    }

    return token_count;
}
int strcut_s(char* str, char sep, char** tokens, size_t max){
