/*
  Projet minishell - Licence 3 Info - PSI 2023
 
  Nom : Delepaut Courtin-Coin
  Prénom : Clement Théo
  Num. étudiant : 22108706 22100975
  Groupe de projet : 15
  Date : 24 / 11 / 2023
 
  Fichier cmd.h: Gestion des processus.
  Dependances: parser.h
  
 */

#ifndef _PROCESSUS_H
#define _PROCESSUS_H

#include <unistd.h>

#include "parser.h"

/*
  Structure de représentation d'une commande à lancer.

  pid       : Identifiant du processus
  status    : Statut du processus
  stdin, stdout, stderr : Descripteurs de fichiers pour les entrées/sorties standard
  wait      : Attendre le processus en avant-plan (1) ou non (0)
  fdclose   : Tableau des descripteurs de fichiers à fermer
  path      : Chemin de l'exécutable
  argv      : Tableau des arguments de la commande
  next      : Commande suivante
  next_success : Commande à exécuter en cas de succès
  next_failure : Commande à exécuter en cas d'échec
*/
typedef struct cmd_t {
  pid_t pid;
  int status;
  int stin, stout, sterr;
  int wait;
  char* path;
  char* argv[MAX_CMD_SIZE];
  int fdclose[MAX_CMD_SIZE];
  struct cmd_t* next;
  struct cmd_t* next_success;
  struct cmd_t* next_failure;
} cmd_t;

/*
  Lancer la commande en fonction des attributs de
  la structure et initialiser les champs manquants.
 
  On crée un nouveau processus, on détourne
  éventuellement les entrées/sorties.
  On conserve le PID dans la structure du processus
  appelant (le minishell).
  On attend si la commande est lancée en "avant-plan"
  et on initialise le code de retour.
  On rend la main immédiatement sinon.
 
  p : un pointeur sur la structure contenant les
      informations pour l'exécution de la commande.
 
  Retourne 0 ou un code d'erreur.
 */
int exec_cmd(cmd_t* p);

/*
  Initialiser une structure cmd_t avec les
  valeurs par défaut.
 
  p : un pointeur sur la structure à initialiser
 
  Retourne 0 ou un code d'erreur.
 */
/**
 * @brief Initialise la structure cmd entré en paramètre
 * 
 * @param p La structure a initialiser
 * @return 0 quand terminer
 */
int init_cmd(cmd_t* p);

/*
  Remplit le tableau de commandes en fonction du contenu
  de tokens.
    Ex : {"ls", "-l", "|", "grep", "^a", NULL} =>
         {{path = "ls",
           argv = {"ls", "-l", NULL},
           bg = 1,
           ...},
          {path = "grep",
           argv = {"grep", "^a", NULL},
           bg = 0,
           ...}}
  tokens : le tableau des éléments de la ligne de
           commandes (peut être modifié)
  cmds : le tableau dans lequel sont stockées les
         différentes structures représentant les
         commandes.
  max : le nombre maximum de commandes dans le tableau

  Retourne 0 ou un code d'erreur.
 */
int parse_cmd(char* tokens[], cmd_t* cmds, size_t max);

/**
 * @brief Ajoute le descripteur de fichier (fd) à la liste des descripteurs à fermer.
 * 
 * @param fdclose Tableau des descripteurs à fermer.
 * @param fd Le descripteur de fichier à ajouter.
 */
void add_fd(int *fdclose, int fd);

/**
 * @brief Fusionne les tableaux de descripteurs à fermer.
 * 
 * @param dest Tableau de destination pour la fusion.
 * @param src Tableau source à fusionner.
 */
void merge_fdclose(int *dest, int *src);


#endif

