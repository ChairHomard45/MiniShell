/*
  Projet minishell - Licence 3 Info - PSI 2023
 
  Nom :
  Prénom :
  Num. étudiant :
  Groupe de projet :
  Date :
 
  Gestion des processus (implémentation).
 
 */

#include "cmd.h"
#include "builtin.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * @brief Lancer la commande en fonction des attributs de la structure et initialiser les champs manquants
 *
 * @param p un pointeur sur la structure contenant les informations pour l'exécution de la commande.
 * @return 0 ou code d'erreur quand terminer
 */
int exec_cmd(cmd_t* p) {
    int status = 0;

    if (p->stin != 0) {
        dup2(p->stin, STDIN_FILENO); // Redirect input
        close(p->stin); // Close the file descriptor
    }

    if (p->stout != 1) {
        dup2(p->stout, STDOUT_FILENO); // Redirect output
        close(p->stout); // Close the file descriptor
    }

    if (p->sterr != 2) {
        dup2(p->sterr, STDERR_FILENO); // Redirect standard error
        close(p->sterr); // Close the file descriptor
    }
	if (strcmp(p->path, "exit") == 0) {
        		status = exit_shell(0, p->sterr);
        		return status;
        }else if( strcmp(p->path, "cd") == 0 ){
        	status = cd(p->argv[0], p->sterr);
        	return status;
        }
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            if (is_builtin(p->path)) {
   		// If it's a built-in command, execute the built-in functionality
        			status = builtin(p);
        			printf("Command Executed: %s %s\n", p->path, p->argv[0]);
        			return status;
            } else {
                // Close file descriptors before executing the command
                for (int i = 0; i < MAX_CMD_SIZE; i++) {
                   if (p->fdclose[i] != -1) {
                      close(p->fdclose[i]);
                    }
                }
                // Execute non-built-in commands using execvp
                if (execvp(p->path, p->argv) == -1) {
                    perror("Command execution failed");
                    exit(EXIT_FAILURE);
                }
            }
        } else if (pid < 0) {
            // Fork failed
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else {
            // Parent process
            waitpid(pid, &status, 0);
        }

    return status; // Return the status of command execution
}

/**
 * @brief Initialise la structure cmd entré en paramètre
 *
 * @param p La structure a initialiser
 * @return 0 quand terminer
 */
 
int init_cmd(cmd_t* p) {
  if (p == NULL) {
        return 1; // retourne 1 si le point p est null
    }

    // initialise les champs de cmd_t avec des valeurs par default
    p->pid = 0;
    p->status = 0;
    p->stin = 0;
    p->stout = 1;
    p->sterr = 2;
    p->wait = 0; //initialise le bg processus par default
    p->path = NULL;
    p->next = NULL;
    p->next_success = NULL;
    p->next_failure = NULL;

    //initialise argv et fdclose tableau avec NULL et 0.
    for (int i = 0; i < MAX_CMD_SIZE; ++i) {
        p->argv[i] = NULL;
        p->fdclose[i] = 0;
    }

    return 0; // Returne 0 comme ré
}



/**
 * @brief La chaine passée en argument est-elle un séparateur ?
 *
 * @param tok La chaine de caractères à tester
 * @return 1 si tok est un séparateur
 *         0 sinon
 */
int is_sep(char * tok) {
  if (!tok) return 0;
  switch (tok[0])
  {
  case ';':
    return 1;
  case '!':
    return 1;
  case '|': // détecte | et ||
    return 1;
  case '&': // détecte & et &&
    return 1;
  case '>': // détecte >, >> et >&2
    return 1;
  case '<': // détecte <, <<, <<<
    return 1;
  case '2': // détecte 2>, 2>>, 2>&1
    if (strlen(tok) >= 2 && tok[1] == '>')
      return 1;
    break;
  }
  return 0;
}


/**
 * @brief la chaine passée en argument est-elle un séparateur de redirection ?
 *
 * @param tok la chaine de caractères à tester
 * @return 1 si tok est une redirection simple
 *         0 si tok est un pipe ou & ou && ou || ou ;
 */
int is_red(char * tok) {
  switch (tok[0])
  {
  case ';':
    return 0;
  case '|': // détecte | et ||
    return 0;
  case '&': // détecte & et &&
    return 0;
  default :
    return 1;
  }
}




/**
 * @brief remplis le tableau de commandes en fonction du contenu de tokens.
 *
 * @param tokens le tableau des éléments de la ligne de commandes (peut être modifié)
 * @param cmds le tableau dans lequel sont stockés les différentes structures représentant le commandes.
 * @param max le nombre maximum de commandes dans le tableau
 * @return 0 ou code d'erreur quand terminer
 */
int parse_cmd(char* tokens[], cmd_t* cmds, size_t max) {
        int finproc = 0, fd;
    for (size_t i = 0; i < max; i++) {
        if (i >= MAX_CMD_SIZE || tokens[i] == NULL) break;
        if (!is_builtin(tokens[i])) {
            printf("Commande invalide\n");
            return 1;
        } else {
            cmds[finproc].pid = finproc;
            cmds[finproc].status = 0;
            cmds[finproc].path = tokens[i];
            cmds[finproc].next = &cmds[finproc + 1];
            int j = 0;
            for (; i < MAX_CMD_SIZE && tokens[i] != NULL && !is_sep(tokens[i]); i++) {
                cmds[finproc].argv[j] = tokens[i];
                j++;
            }
            cmds[finproc].argv[j] = NULL;
            if (i >= MAX_CMD_SIZE || tokens[i] == NULL) break;


            find_sep: //si ce point est atteint tokens[i] est un séparateur
      switch (tokens[i][0])
      {
      case ';': //on garde les parametres actuels
        break;
      case '|': // détecte | et ||
        if (!strcmp(tokens[i], "||")) {
          cmds[finproc].next_failure = &cmds[finproc+1]; //ajoute une suite si echec
          cmds[finproc].next = NULL; //supprime la suite inconditionnelle
        }
        else { // |
          if (pipe(cmds[finproc+1].fdclose)) { //crée un pipe
            printf("Erreur pipe impossible\n");
            return 1;
          }
          cmds[finproc].stout = cmds[finproc+1].fdclose[1]; //ajoute la sortie processus
          cmds[finproc+1].stin = cmds[finproc+1].fdclose[0]; //ajoute l'entrée au prochain processus
          cmds[finproc].fdclose[0] = cmds[finproc+1].fdclose[1];
        }
        break;
      case '&': // détecte & et &&
        if (!strcmp(tokens[i], "&&")) {
          cmds[finproc].next_success = &cmds[finproc+1]; //ajoute une suite si succes
          cmds[finproc].next = NULL; //supprime la suite inconditionnelle
        }
        else { // &
          char * r = substenv("RECYCLE", envar);
          if (r) {
            cmds[finproc].fdclose[0] = open(r, O_WRONLY | O_CREAT | O_TRUNC, 0640);
            free(r);
          } else {
            r = default_recycle;
            cmds[finproc].fdclose[0] = open(r, O_WRONLY | O_CREAT | O_TRUNC, 0640);
            export("RECYCLE", r, 2);
          }
          cmds[finproc].next = &cmds[finproc+1];
          cmds[finproc].wait = 0;
          if (cmds[finproc].stout == 1)
            cmds[finproc].stout = cmds[finproc].fdclose[0];
          if (cmds[finproc].sterr == 2)
            cmds[finproc].sterr = cmds[finproc].fdclose[0];
        }
        break;
      case '>': // détecte >, >> et >&2
        if (cmds[finproc].stout > 2) close(cmds[finproc].stout);
        if (!strcmp(tokens[i], ">")) {
          if (i+1<MAX_CMD_SIZE && tokens[i+1]!=NULL) {
            fd = open(tokens[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0640);
            if (fd != -1){
              cmds[finproc].stout = fd;
              cmds[finproc].fdclose[0] = fd;
              i++;
              break;
            } else printf("Le fichier %s n'est pas valide\n", tokens[i+1]);
          } else printf("Argument fichier manquant\n");
          return 1;
        }
        if (!strcmp(tokens[i], ">>")) {
          if (i+1<MAX_CMD_SIZE && tokens[i+1]!=NULL) {
            fd = open(tokens[i+1], O_WRONLY | O_CREAT | O_APPEND, 0640);
            if (fd != -1) {
              cmds[finproc].stout = fd;
              cmds[finproc].fdclose[0] = fd;
              i++;
              break;
            } else printf("Le fichier %s n'est pas valide\n", tokens[i+1]);
          } else printf("Argument fichier manquant\n");
          return 1;
        }
        if (!strcmp(tokens[i], ">&2")) {
          cmds[finproc].stout = cmds[finproc].sterr;
        }
        break;
      case '<': // détecte <, <<, <<<
        if (cmds[finproc].stin > 2) close(cmds[finproc].stin);
        if (!strcmp(tokens[i], "<")) {
          if (i+1<MAX_CMD_SIZE && tokens[i+1]!=NULL) {
            fd = open(tokens[i+1], O_RDONLY, 0640);
            if (fd != -1) {
              cmds[finproc].stin = fd;
              cmds[finproc].fdclose[0] = fd;
              i++;
              break;
            } else printf("Le fichier %s n'est pas valide\n", tokens[i+1]);
          } else printf("Argument fichier manquant\n");
        } else printf("<< et <<< ne sont pas pris en charge\n");
        return 1;
      case '2': // détecte 2>, 2>>, 2>&1
        if (cmds[finproc].sterr > 2) close(cmds[finproc].sterr);
        if (!strcmp(tokens[i], "2>")) {
          if (i+1<MAX_CMD_SIZE && tokens[i+1]!=NULL) {
            fd = open(tokens[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0640);
            if (fd != -1){
              cmds[finproc].sterr = fd;
              cmds[finproc].fdclose[0] = fd;
              i++;
              break;
            } else printf("Le fichier %s n'est pas valide\n", tokens[i+1]);
          } else printf("Argument fichier manquant\n");
          return 1;
        }
        if (!strcmp(tokens[i], "2>>")) {
          if (i+1<MAX_CMD_SIZE && tokens[i+1]!=NULL) {
            fd = open(tokens[i+1], O_WRONLY | O_CREAT | O_APPEND, 0640);
            if (fd != -1) {
              cmds[finproc].sterr = fd;
              cmds[finproc].fdclose[0] = fd;
              i++;
              break;
            } else printf("Le fichier %s n'est pas valide\n", tokens[i+1]);
          } else printf("Argument fichier manquant\n");
          return 1;
        }
        if (!strcmp(tokens[i], "2>&1")) {
          cmds[finproc].sterr = cmds[finproc].stout;
          break;
        }
      }
      if (is_red(tokens[i])) {
        i++;
        if (i<MAX_CMD_SIZE && tokens[i]){
          goto find_sep;
        }
        //else
          break;
      }




            finproc++;
        }
    }
    return 0;  // Return 0 or handle error codes accordingly
}
