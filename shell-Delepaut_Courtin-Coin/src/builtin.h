/*
  Projet minishell - Licence 3 Info - PSI 2023
 
  Nom : Delepaut Courtin-Coin
  Prénom : Clement Théo
  Num. étudiant : 22108706 22100975
  Groupe de projet : 15
  Date : 24 / 11 / 2023
 
  Fichier builtin.h : Gestion des commandes internes du minishell.
  Dépendances : cmd.h

 */
#ifndef _BUILTIN_H
#define _BUILTIN_H

#include "cmd.h"

#define MAX_HISTORY_SIZE 100

// Affiche la description de la commande si elle est dans le tableau
/**
 * @brief Affiche la description de toutes les commandes présentes dans le mini-shell
*/
void print_command_description();

/**
 * @brief Ajouter une commande a history
 * 
 * 
 * @param cmd commande a ajouté
*/
void add_to_history(const char* cmd);


/**
 * @brief Afficher l'historique des commandes
 * 
*/
void print_history();

/**
 * @brief Libère la mémoire alloué a history
 *  free toutes les case de history
*/
void freehistory();


int execute_previous_command(char* input);



/*
 La chaîne passée représente-t-elle une commande
 interne ?
 
 cmd : chaîne de caractères.
 
 Retourne 1 si la chaîne passée désigne une
 commande interne (cd, exit, ...)
 */
int is_builtin(const char* cmd);

/*
  Exécute la commande interne
 
  proc : process_t dont le contenu a été initialisé
         au moment du parsing
 
  Retourne 0 si tout s'est bien passé, la valeur de
  retour de la commande ou -1 si la commande n'est
  pas reconnue.
 */
int builtin(cmd_t* cmd);

/*
  Change directory : change le répertoire de travail
  courant du minishell.

  path : le chemin vers lequel déplacer le CWD
  fderr : le descripteur de la sortie d'erreur pour
          affichage éventuel (erreur du cd)
  
  Retourne le code de retour de l'appel système.
 */
int cd(const char* path, int fderr);

/*
  Ajout/modification d'une variable d'environnement.
 
  var : nom de la variable
  value : valeur à lui donner
  fderr : le descripteur de la sortie d'erreur pour
          affichage éventuel
 
  Retourne le code de retour de l'appel système.
 */
int export(const char* var, const char* value, int fderr);

/*
  Quitter le minishell
 
  ret : code de retour du minishell
  fdout : descripteur de la sortie standard pour
          l'affichage éventuel d'un message de sortie
 
  Retourne un code d'erreur en cas d'échec.
 */
int exit_shell(int ret, int fderr);


/**
 * @brief Enlève la valeur a associé a la variable 
 * 
 * @param var variable a 
 * @return 0 si succes, 1 sinon
 */
int unset(const char *var);

#endif
