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

int trim(char* str) {
  if (str == NULL) {
    	return -1; // Code d'erreur pour un paramètre invalide
	}

	int len = strlen(str);

	// Recherche du premier caractère non espace au début
	int start = 0;
	while (start < len && str[start] == ' ') {
    	start++;
	}

	// Recherche du premier caractère non espace à la fin
	int end = len - 1;
	while (end >= 0 && str[end] == ' ') {
    	end--;
	}

	// Si start est supérieur à end, la chaîne ne contient que des espaces
	if (start > end) {
    	str[0] = '\0'; // La chaîne devient vide
	} else {
    	// Déplacement des caractères pour éliminer les espaces au début
    	for (int i = 0; i <= end - start; i++) {
        	str[i] = str[start + i];
    	}
    	str[end - start + 1] = '\0'; // Terminaison de la chaîne résultante
	}

	return strlen(str);
}

int clean(char* str) {

}

int separate_s(char* str, char* s, size_t max) {

}

int substenv(char* str, size_t max) {

}

int strcut(char* str, char sep, char** tokens, size_t max) {

}

// Optionnel
int strcut_s(char* str, char sep, char** tokens, size_t max) {

}
