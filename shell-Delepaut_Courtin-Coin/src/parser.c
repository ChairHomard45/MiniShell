/*
  Projet minishell - Licence 3 Info - PSI 2023
  
  Nom : Delepaut
  Prénom : Clement
  Num. étudiant : 22108706
  Groupe de projet : 15
  Date :
 
  Parsing de la ligne de commandes utilisateur (implémentation).
 
 */

#include <stddef.h>
//#include <ctype.h>//Pour la version avec isspace

int trim(char* str) {
      if (str == NULL) {
    	return -1; // Code d'erreur pour un parametre invalide
	}
	if (strlen(str) == 0){
		return 0;
	}

	int start = 0;
	

	while (str[start] == ' '||str[start] == '\t'||str[start] == '\n'){start++;} // Version sans isspace
	//while (isspace(str[start])){start++;} // Version avec isspace
	if(start){
		char * tmp = strdup(str);
    	strcpy(str, (char *) (tmp+start)); // on supprime les espaces
    	free(tmp);
	}
	
	int end = strlen(str) - 1;
	while(str[end] == ' '||str[end] == '\t'||str[end] == '\n'){str[end]='\0';end--;} // Sans Isspace
	//while (isspace(str[end])){str[end]='\0';end--;} // Avec isspace
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
                // Variable de la forme ${VAR}
                Lecture += 2; // passe le "{"
                size_t varStart = Lecture;
                while (str[Lecture] != '}' && str[Lecture] != '\0') {
                    Lecture++;
                }
                if (str[Lecture] == '}') {
                    // Variable trouvé
                    str[Lecture] = '\0'; // On termine temporairement la chaine
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
                    // On récupère ou l'on était dans la chaine après la variable
                    str[Lecture] = '}';
					Lecture++;
                }
            } else {
                // Variable de la forme $VAR
                Lecture++;
                size_t varStart = Lecture;
                while (str[Lecture] != ' ' && str[Lecture] != '\0') {
                    Lecture++;
                }
                // Copie le nom de la variable
                size_t varLen = Lecture - varStart;
                if (varLen > 0) {
                    str[Lecture] = '\0'; // On termine temporairement la chaine
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
                    // On récupère ou l'on était dans la chaine après la variable
                    str[Lecture] = (str[Lecture] == ' ') ? ' ' : '\0';
					Lecture++;
                }
            }
        } else {
            // Copie les caractères normaux
            out[Ecriture++] = str[Lecture++];
        }
    }

    // Termine la chaine de retour
    out[Ecriture] = '\0';

    // Copie la chaine resultant a str if elle rentre 
    if (Ecriture < max) {
        strcpy(str, out);
    }else {
        //Si c'est trop long alors on la coupe
        str[max - 1] = '\0';
    }

    free(out);

    return 0;
}

int strcut(char* str, char sep, char** tokens, size_t max) {
    if (str == NULL || tokens == NULL || max == 0) {
        return -1; // Error: Paramètre Invalide
    }

    size_t token_count = 0;
    char* token_start = str;
    size_t str_len = strlen(str);

    for (size_t i = 0; i <= str_len; i++) {
        if (str[i] == sep || str[i] == '\0') {
            // Separateur trouver ou fin de chaine, creation de token
            size_t token_length = i - (token_start - str);
            tokens[token_count] = (char*)malloc(token_length + 1);
            if (tokens[token_count] == NULL) {
                // Handle memory allocation failure
                return -1;
            }
            strncpy(tokens[token_count], token_start, token_length);
            tokens[token_count][token_length] = '\0';
            token_count++;
            token_start = str + i + 1; //Avance au caractère suivant après le separateur

            if (token_count == max - 1) {
                // Si on a atteint max-1 token on en crée un dernier en tant que null et on return le compte
                tokens[token_count] = NULL;
                return token_count;
            }
        }
    }

    return token_count;
}

// Optionnel
int strcut_s(char* str, char sep, char** tokens, size_t max) {
    if (str == NULL || tokens == NULL || max == 0) {
        return -1; // Error: Paramètre invalide
    }
}

