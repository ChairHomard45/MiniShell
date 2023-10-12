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
if (str == NULL) {
    	return -1; // Code d'erreur pour un paramètre invalide
	}

	int len = strlen(str);
	bool is_space = false; // Indique si le dernier caractère était un espace
	int result_index = 0; // Indice pour la nouvelle chaîne résultante

	for (int i = 0; i < len; i++) {
    	if (str[i] == ' ') {
        	if (!is_space) {
            	// Ajoute un seul espace si ce n'était pas un doublon
            	str[result_index] = ' ';
            	result_index++;
        	}
        	is_space = true;
    	} else {
        	// Ajoute le caractère non espace à la nouvelle chaîne
        	str[result_index] = str[i];
        	result_index++;
        	is_space = false;
    	}
	}

	// Termine la chaîne résultante
	str[result_index] = '\0';

	return strlen(str);

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
