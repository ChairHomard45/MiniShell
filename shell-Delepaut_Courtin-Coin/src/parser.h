/*
  Projet minishell - Licence 3 Info - PSI 2023
 
  Nom :
  Prénom :
  Num. étudiant :
  Groupe de projet :
  Date :
 
  Parsing de la ligne de commandes utilisateur.
 
 */

#ifndef _PARSER_H
#define _PARSER_H

#define MAX_LINE_SIZE   1024
#define MAX_CMD_SIZE    256

/*
  Voir le TP sur les chaînes de caractères.
  Commentaires à compléter.
*/
/**
 * @brief On veut supprimer tous les espaces au début
 * et à la fin de la chaine de caractères.
 * 
 * On compte le nombre d'espcaes au début de la chaine et
 * on les supprime.
 * On part de la fin de la chaine et on décrémente tant
 * qu'il y a un espace, et on remplace l'espace à cet indice
 * par '\0'
 * 
 * @param str une chaine de caractères
 * @return la taille de la chaine de caractère ou un code d'erreur.
*/
int trim(char* str);
/**
 * @brief On veut supprimer tous les espaces en double dans la chaine de caractères.
 * 
 * On avance dans la chaine tant qu'il n'y pas d'espace, dès qu'on en trouve un
 * On vérifie si il y a des doublons après et on les supprimes.
 * On continue tant qu'on est pas a la fin de la chaine '\0'
 * 
 * @param str une chaine de caractères
 * @return la taille de la chaine de caractère ou un code d'erreur.
*/
int clean(char* str);
/**
 * @brief Ajoute des espaces autour de toutes les occurrences des caractères
 * d'une chaîne lorsqu'il n'y a pas d'espace, et sans ajouter d'espace au
 * début ou à la fin. L'opération s'arrête avant que la capacité maximale
 * de la chaine soit dépassée.
 * 
 * @param str la chaine de caractères qu'on doit traiter
 * @param s le caractère autour duquel on doit ajouter des espaces
 * @param max taille maximale de la chaine de caractère
 * @return int la longeur de la chaine 
 */
int separate_s(char* str, char* s, size_t max);
/**
 * @brief  remplace les mots du type $NOM ou ${NOM} par la
 * variable d’environnement de nom correspondant si elle existe.
 * Si elle n’existe pas, la substitution n’est pas effectuée.
 * La chaîne finale ne dépassera pas cette taille (\0 compris)
 * 
 * @param srch la chaine de caractères à vérifier
 * @param env tableau des variables d'environnement
 * @return char* la chaine après substitution ou NULL
 */
int substenv(char* str, size_t max);
/**
 * @brief Découpe une chaine de caractères en différents "mots"
 * dans un tableau de chaines de caractères à chaque occurence
 * d'un caractère sep.
 * 
 * @param str la chaine de caractères que l'on veut traiter
 * @param sep le caractère à partir duquel on sépare les mots
 * @param tokens le tableau de mots
 * @param max la taille maximale de tokens
 * @return int 
 */
int strcut(char* str, char sep, char** tokens, size_t max);

// Optionnel
int strcut_s(char* str, char sep, char** tokens, size_t max);
#endif
