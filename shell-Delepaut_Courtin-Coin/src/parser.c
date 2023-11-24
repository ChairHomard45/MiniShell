/*
  Projet minishell - Licence 3 Info - PSI 2023
 
  Nom : Delepaut Courtin-Coin
  Prénom : Clement Théo
  Num. étudiant : 22108706 22100975
  Groupe de projet : 15
  Date : 24 / 11 / 2023
 
  Fichier parser.c: Parsing de la ligne de commandes utilisateur (implémentation).

 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Fonction pour supprimer les espaces au début et à la fin d'une chaîne de caractères
int trim(char* str) {
    if (str == NULL) {
        return -1; // Code d'erreur pour un paramètre invalide
    }
    if (strlen(str) == 0) {
        return 0;
    }

    int start = 0;

    // Supprimer les espaces au début de la chaîne
    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\n') {
        start++;
    }

    // Si des espaces ont été trouvés, les supprimer de la chaîne
    if (start) {
        char* tmp = strdup(str);
        strcpy(str, (char*)(tmp + start)); // On supprime les espaces
        free(tmp);
    }

    int end = strlen(str) - 1;

    // Supprimer les espaces à la fin de la chaîne
    while (str[end] == ' ' || str[end] == '\t' || str[end] == '\n') {
        str[end] = '\0';
        end--;
    }

    return strlen(str);
}

// Fonction pour supprimer les doublons d'espaces dans une chaîne de caractères
int clean(char* str) {
    if (str == NULL) {
        return -1; // Gérer le cas où la chaîne est nulle
    }
    if (strlen(str) == 0) {
        return 0;
    }

    int Ecriture = 0;
    int Lecture = 0;
    bool LastCharacter = false;

    // Supprimer les doublons d'espaces
    while (Lecture < strlen(str)) {
        if (!(str[Lecture] == ' ' || str[Lecture] == '\t' || str[Lecture] == '\n')) {
            str[Ecriture] = str[Lecture];
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

// Fonction pour ajouter des espaces autour des caractères spéciaux dans une chaîne de caractères
int separate_s(char* str, const char* s, size_t max) {
    if (str == NULL || s == NULL || max == 0) {
        return -1; // Indiquer une erreur en raison de paramètres invalides
    }

    size_t newLen = 0;
    int idx_str = 0;

    // Créer un tampon temporaire
    char* tmp = (char*)malloc(max);

    if (tmp == NULL) {
        return -1; // Erreur d'allocation
    }

    while (str[idx_str] != '\0' && newLen < max - 1) {
        char character = str[idx_str];

        // Vérifier si le caractère est spécial
        if (strchr(s, character) != NULL) {
            // Gérer les caractères spéciaux
            switch (character) {
                case ';':
                    tmp[newLen++] = ' ';
                    tmp[newLen++] = character;
                    tmp[newLen++] = ' ';
                    break;
                //case '!':
                //    tmp[newLen++] = ' ';
                //   tmp[newLen++] = character;
                //    tmp[newLen++] = ' ';
                //   break;
                case '&':
                    if (str[idx_str + 1] == '&') {
                        // Gérer le cas '&&'
                        tmp[newLen++] = ' ';
                        tmp[newLen++] = character;
                        tmp[newLen++] = character;
                        tmp[newLen++] = ' ';
                        idx_str++; // Ignorer un caractère pour '&'
                    } /*else if (str[idx_str + 1] == '>') {
                        // Gérer le cas '&>'
                        tmp[newLen++] = ' ';
                        tmp[newLen++] = character;
                        tmp[newLen++] = ' ';
                        idx_str++; // Ignorer un caractère pour '>'
                    } */else {
                        // Gérer le cas '&'
                        tmp[newLen++] = ' ';
                        tmp[newLen++] = character;
                        tmp[newLen++] = ' ';
                    }
                    break;
                // Gérer d'autres cas de manière similaire avec des espaces avant et après
                case '|':
                    if (str[idx_str + 1] == '|') {
                        // Gérer le cas '||'
                        tmp[newLen++] = ' ';
                        tmp[newLen++] = character;
                        tmp[newLen++] = character;
                        tmp[newLen++] = ' ';
                        idx_str++; // Ignorer un caractère pour '|'
                    } else {
                        // Gérer le cas '|'
                        tmp[newLen++] = ' ';
                        tmp[newLen++] = character;
                        tmp[newLen++] = ' ';
                    }
                    break;
                case '<':
                    // Gérer le cas '<'
                    tmp[newLen++] = ' ';
                    tmp[newLen++] = character;
                    tmp[newLen++] = ' ';
                    break;
                case '>':
                    if (str[idx_str + 1] == '>') {
                        // Gérer le cas '>>'
                        tmp[newLen++] = ' ';
                        tmp[newLen++] = character;
                        tmp[newLen++] = character;
                        tmp[newLen++] = ' ';
                        idx_str++; // Ignorer un caractère pour '>'
                    } else if (str[idx_str + 1] == '&' && str[idx_str + 2] == '2') {
                        // Gérer le cas '>&2'
                        tmp[newLen++] = ' ';
                        tmp[newLen++] = character;
                        tmp[newLen++] = str[idx_str + 1];
                        tmp[newLen++] = str[idx_str + 2];
                        tmp[newLen++] = ' ';
                        idx_str += 2; // Ignorer les caractères pour '>&2'
                    } else {
                        // Gérer le cas '>'
                        tmp[newLen++] = ' ';
                        tmp[newLen++] = character;
                        tmp[newLen++] = ' ';
                    }
                    break;
                case '2':
                    if (str[idx_str + 1] == '>' && str[idx_str + 2] == '>') {
                        // Gérer le cas '2>>'
                        tmp[newLen++] = ' ';
                        tmp[newLen++] = character;
                        tmp[newLen++] = str[idx_str + 1];
                        tmp[newLen++] = str[idx_str + 2];
                        idx_str += 2; // Ignorer deux caractères pour '>>'
                    } else if (str[idx_str + 1] == '>' && str[idx_str + 2] == '&' && str[idx_str + 3] == '1') {
                        // Gérer le cas '2>&1'
                        tmp[newLen++] = ' ';
                        tmp[newLen++] = character;
                        tmp[newLen++] = str[idx_str + 1];
                        tmp[newLen++] = str[idx_str + 2];
                        tmp[newLen++] = str[idx_str + 3];
                        idx_str += 3; // Ignorer les caractères pour '2>&1'
                    } else if (str[idx_str + 1] == '>') {
                        // Gérer le cas '2>'
                        tmp[newLen++] = ' ';
                        tmp[newLen++] = character;
                        tmp[newLen++] = str[idx_str + 1];
                        idx_str++; // Ignorer un caractère pour '>'
                    }
                    break;

                default:
                    // Ne rien faire
                    break;
            }
        } else { // Copier le caractère non spécial dans le tampon temporaire
            tmp[newLen] = character;
            newLen++;
        }

        idx_str++;
    }

    // Null-terminer le tampon temporaire
    tmp[newLen] = '\0';

    // Copier la chaîne modifiée dans la chaîne d'origine
    strcpy(str, tmp);

    // Libérer le tampon temporaire
    free(tmp);

    return 0; // Succès
}

// Fonction pour substituer les variables d'environnement dans une chaîne de caractères
int substenv(char* str, size_t max) {
    if (str == NULL || max <= 0) {
        printf("Paramètres invalides\n");
        return -1; // Paramètres invalides
    }

    size_t strLen = strlen(str);

    for (size_t i = 0; i < strLen; ++i) {
        if (str[i] == '$' && i + 1 < strLen) {
            if (str[i + 1] == '{') {
                size_t start = i + 2;
                size_t end = start;

                // Trouver la fin de la variable d'environnement
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

                        // Vérifier si l'espace est suffisant pour substituer la variable
                        if (envValueLen <= max - strLen + replaceLen) {
                            memmove(str + i + envValueLen, str + end + 1, strLen - end);
                            memcpy(str + i, envValue, envValueLen);
                            strLen = strLen - replaceLen + envValueLen;
                            i += envValueLen - 1;
                        } else {
                            printf("Espace insuffisant pour substituer la variable\n");
                            return -1; // Espace insuffisant
                        }
                    } else {
                        printf("Variable d'environnement '%s' non trouvée\n", varName);
                        // Gérer le cas où la variable d'environnement n'est pas trouvée si nécessaire
                    }
                }
            } else {
                size_t start = i + 1;
                size_t end = start;

                // Trouver la fin de la variable d'environnement
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

                    // Vérifier si l'espace est suffisant pour substituer la variable
                    if (envValueLen <= max - strLen + replaceLen) {
                        memmove(str + i + envValueLen, str + end, strLen - end);
                        memcpy(str + i, envValue, envValueLen);
                        strLen = strLen - replaceLen + envValueLen;
                        i += envValueLen - 1;
                    } else {
                        printf("Espace insuffisant pour substituer la variable\n");
                        return -1; // Espace insuffisant
                    }
                } else {
                    printf("Variable d'environnement '%s' non trouvée\n", varName);
                    // Gérer le cas où la variable d'environnement n'est pas trouvée si nécessaire
                }
            }
        }
    }

    str[strLen] = '\0';
    return strLen;
}

// Fonction pour supprimer les guillemets d'une chaîne
void stripQuotes(char* str) {
    // Récupère la longueur de la chaîne
    size_t len = strlen(str);

    // Vérifie si la chaîne a au moins deux caractères et commence et se termine par des guillemets
    if (len >= 2 && str[0] == '\"' && str[len - 1] == '\"') {
        // Supprime le premier et le dernier caractères (les guillemets)
        memmove(str, str + 1, len - 2);
        str[len - 2] = '\0';
    }
}

// Fonction pour découper une chaîne en tokens en utilisant un séparateur donné
// Les tokens peuvent être entourés de guillemets pour inclure des espaces
int strcut(char* str, char sep, char** tokens, size_t max) {
    // Compteur de tokens
    int tokenCount = 0;
    
    // Récupère le premier token en utilisant strtok
    char* token = strtok(str, " ");

    // Boucle tant qu'il y a des tokens et que le nombre maximum n'est pas atteint
    while (token != NULL && tokenCount < max) {
        // Vérifie si le token est entre guillemets
        if (token[0] == '\"') {
            // Alloue de la mémoire pour le token et copie le contenu entre guillemets
            tokens[tokenCount] = strdup(token);
            if (tokens[tokenCount] == NULL) {
                fprintf(stderr, "Erreur d'allocation de mémoire\n");
                exit(EXIT_FAILURE);
            }

            // Ajoute les tokens suivants jusqu'à ce qu'un guillemet de fermeture soit trouvé
            while (token[strlen(token) - 1] != '\"') {
                // Récupère le token suivant
                token = strtok(NULL, " ");
                if (token == NULL) {
                    fprintf(stderr, "Erreur : guillemet de fermeture manquant\n");
                    exit(EXIT_FAILURE);
                }

                // Concatène l'espace et le prochain token
                char* temp = strdup(" ");
                if (temp == NULL) {
                    fprintf(stderr, "Erreur d'allocation de mémoire\n");
                    exit(EXIT_FAILURE);
                }
                tokens[tokenCount] = realloc(tokens[tokenCount], strlen(tokens[tokenCount]) + strlen(temp) + strlen(token) + 1);
                strcat(tokens[tokenCount], temp);
                strcat(tokens[tokenCount], token);
                free(temp);
            }
            
            // Supprime les guillemets des tokens entre guillemets
            stripQuotes(tokens[tokenCount]);
        } else {
            // Token non entre guillemets
            tokens[tokenCount] = strdup(token);
            if (tokens[tokenCount] == NULL) {
                fprintf(stderr, "Erreur d'allocation de mémoire\n");
                exit(EXIT_FAILURE);
            }
        }

        // Récupère le token suivant
        token = strtok(NULL, " ");
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
