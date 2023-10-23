
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
    if(str == NULL || s == NULL || max <= 0){printf("Problème paramètres");return -1;}//Problème paramètre

    int indexLec = 0;
    int indexEcrit = 0;
    char* out = (char*) calloc(max,sizeof(char));

    while(str[indexLec] != '\0' && indexLec <= max - 1){
    if(strchr(s,str[indexLec])){
        if(str[indexLec] == ';'){
            if(indexLec!=0){if(str[indexLec-1]!=' ') strcat(out," ");indexEcrit++;}
            strncat(out,str+indexLec,1);
            indexEcrit++;
            if (str[indexLec+1] != ' ' && str[indexLec+1] != '\0') strcat(out, " ");indexEcrit++;
        }else if(str[indexLec] == '!'){
            if(indexLec!=0){if(str[indexLec-1]!=' ') strcat(out," ");indexEcrit++;}
            strncat(out,str+indexLec,1);indexEcrit++;
            if (str[indexLec+1] != ' ' && str[indexLec+1] != '\0') strcat(out, " ");indexEcrit++;
        }else if(str[indexLec] == '|'){
            if(indexLec!=0){if(str[indexLec-1]!=' ' && str[indexLec-1]!='|' ) strcat(out," ");indexEcrit++;}
            strncat(out,str+indexLec,1);indexEcrit++;
            if (str[indexLec+1] != ' ' && str[indexLec+1]!='|' && str[indexLec+1] != '\0') strcat(out, " ");indexEcrit++;
        }else if(str[indexLec] == '&'){
            if(indexLec!=0){if(str[indexLec-1]!=' ' && str[indexLec-1]!='&' ) strcat(out," ");indexEcrit++;}
            strncat(out,str+indexLec,1);indexEcrit++;
            if (str[indexLec+1] != ' ' && str[indexLec+1]!='&' && str[indexLec+1] != '\0') strcat(out, " ");indexEcrit++;
        }else if(str[indexLec] == '\"'){
            if(indexLec!=0){if(str[indexLec-1]!=' ') strcat(out," ");indexEcrit++;}
            strncat(out, str+indexLec, 1);indexEcrit++;
            indexLec++;
            while((str[indexLec]!='\"' || str[indexLec-1]=='\\') && str[indexLec]!='\0'){strncat(out,str+indexLec,1); indexLec++;indexEcrit++;}
            strncat(out, str+indexLec, 1);indexEcrit++;
            indexLec++;
            if (str[indexLec+1] != ' ' && str[indexLec+1] != '\0') strcat(out, " ");indexEcrit++;
        }else if(str[indexLec] == '\\'){
            strncat(out, str+indexLec, 1);indexEcrit++;
            indexLec++;
            strncat(out, str+indexLec, 1);indexEcrit++;
        }else if(str[indexLec] == '>'){
            if(indexLec!=0){if(str[indexLec-1]!=' ') strcat(out," ");indexEcrit++;}
            strncat(out, str+indexLec, 1);indexEcrit++;
            if(str[indexLec+1]== '>'){
                indexLec++;
                strncat(out, str+indexLec, 1);indexEcrit++;
                
            }
            if(str[indexLec+1]== '&'){
                if(str[indexLec+2]!='2') {strcat(out," ");indexEcrit++;}
                else{
                    indexLec++;
                    strncat(out, str+indexLec, 2);indexEcrit+=2;
                    indexLec++;
                }
            }
            if (str[indexLec+1] != ' ' && str[indexLec+1] != '\0') strcat(out, " ");indexEcrit++;
        }else if(str[indexLec] == '<'){
            if(indexLec!=0){if(str[indexLec-1]!=' ') strcat(out," ");indexEcrit++;}
            strncat(out, str+indexLec, 1);indexEcrit++;
            if (str[indexLec+1] == '<') {
                strncat(out, str+indexLec, 1);indexEcrit++;
                indexLec++;
                if (str[indexLec+1] == '<') {
                strncat(out, str+indexLec, 1);indexEcrit++;
                indexLec++;
                }
            }
           if (str[indexLec+1] != ' ' && str[indexLec+1] != '\0') strcat(out, " ");indexEcrit++;
        }else if(str[indexLec] == '2'){
            if (str[indexLec+1] != '>') {
                strncat(out, str+indexLec, 1);indexEcrit++;
            }else{
                if(indexLec!=0){if(str[indexLec-1]!=' ') strcat(out," ");indexEcrit++;}
                strncat(out, str+indexLec, 2);indexEcrit++;
                indexLec++;
                if(str[indexLec+1] == '>'){
                    strncat(out, str+indexLec, 2);indexEcrit+=2;
                    indexLec++;
                }else if(str[indexLec+1] == '&'){
                    if(str[indexLec+1] == '1'){
                        strncat(out, str+indexLec, 2);indexEcrit+=2;
                        indexLec++;
                    }
                }
                if (str[indexLec+1] != ' ' && str[indexLec+1] != '\0') strcat(out, " ");indexEcrit++;
            }
        }else if(str[indexLec] == '$'){
            if(indexLec!=0){if(str[indexLec-1]!=' ') strcat(out," ");indexEcrit++;}
            strncat(out, str+indexLec, 1);indexEcrit++;
            if(str[indexLec+1] == '{'){
                indexLec+=2;
                for(indexLec;((str[indexLec]!='}' || str[indexLec-1]=='\\') && str[indexLec]!='\0'); indexLec++) strncat(out, str+indexLec, 1);indexEcrit++;
                if (str[indexLec+1] != ' ' && str[indexLec+1] != '\0') strcat(out, " ");indexEcrit++;
            }
        }else if(str[indexLec] == ' '){
            if(indexLec!=0){if(str[indexLec-1]!=' ') strcat(out," ");indexEcrit++;}
        }
        }else{strncat(out, str+indexLec, 1);indexEcrit++;}
        indexLec++;
    }
    out[indexEcrit++]='\0';
    if(indexEcrit <= max ){
        strncpy(str, out, max);  // Copy at most 'max' characters from 'out' to 'str'
        str[max] = '\0';
        
    }else{
        strncpy(str, out, max - 1);  // Copy 'max - 1' characters from 'out' to 'str'
        str[max - 1] = '\0'; 
    }
	free(out);
    return strlen(str);

}
int substenv(char* str, size_t max) {
    if (str == NULL || max == 0) {
        return -1; // Error: invalid parameters
    }

    char* result = (char*)malloc(max); // Use malloc instead of calloc
    if (result == NULL) {
        return -1; // Error: memory allocation failed
    }

    size_t Ecriture = 0;
    size_t Lecture = 0;

    while (str[Lecture] != '\0' && Ecriture < max - 1) {
        if (str[Lecture] == '$') {
            size_t varStart = Lecture + 1;  // Skip the '$'
            if (str[varStart] == '{') {
                // Variable of the form ${VAR}
                varStart++; // Skip the "{"
                size_t varEnd = varStart;
                while (str[varEnd] != '}' && str[varEnd] != '\0') {
                    varEnd++;
                }
                if (str[varEnd] == '}') {
                    // Variable found
                    str[varEnd] = '\0'; // Temporarily null-terminate the variable
                    char* envVar = str + varStart;
                    char* envValue = getenv(envVar);
                    str[varEnd] = '}'; // Restore the original string
                    if (envValue != NULL) {
                        size_t envLen = strlen(envValue);
                        if (Ecriture + envLen < max) {
                            strncpy(result + Ecriture, envValue, envLen);
                            Ecriture += envLen;
                        }
                    }
                    Lecture = varEnd + 1;
                } else {
                    // Unmatched '{', treat as a normal character
                    result[Ecriture++] = '$';
                    Lecture++;
                }
            } else {
                // Variable of the form $VAR
                size_t varEnd = varStart;
                while (str[varEnd] != ' ' && str[varEnd] != '\0') {
                    varEnd++;
                }
                str[varEnd] = '\0'; // Temporarily null-terminate the variable
                char* envVar = str + varStart;
                char* envValue = getenv(envVar);
                str[varEnd] = ' '; // Restore the original string
                if (envValue != NULL) {
                    size_t envLen = strlen(envValue);
                    if (Ecriture + envLen < max) {
                        strncpy(result + Ecriture, envValue, envLen);
                        Ecriture += envLen;
                    }
                }
                Lecture = varEnd + 1;
            }
        } else {
            // Copy normal characters
            result[Ecriture++] = str[Lecture++];
        }
    }

    result[Ecriture] = '\0';  // Null-terminate the resulting string

    if (Ecriture < max) {
        strncpy(str, result, max);
        str[Ecriture] = '\0';  // Ensure the original string is null-terminated
    } else {
        strncpy(str, result, max - 1);
        str[max - 1] = '\0';
    }

    free(result);

    return 0;
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