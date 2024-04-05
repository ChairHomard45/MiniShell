/*
  Projet minishell - Licence 3 Info - PSI 2023
 
  Nom : Delepaut Courtin-Coin
  Prénom : Clement Théo
  Num. étudiant : 22108706 22100975
  Groupe de projet : 15
  Date : 24 / 11 / 2023
 
  Fichier cmd.c: Gestion des processus (implémentation).
  Dépendances : cmd.h builtin.h

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
    
    if(p->path == NULL || p->argv == NULL){
    	if (write(STDERR_FILENO, "Erreur: path et argv NULL", strlen("Erreur: path et argv NULL")) == -1) {
            perror("Erreur ecriture to fderr");
            // Gérer l'erreur selon les besoins
            exit(EXIT_FAILURE);
        }
    	return -1;
    }
    
    pid_t pid = fork();
        
    if (pid == 0) {
        // Processus fils
        if (p->stin != STDIN_FILENO) {
            if (dup2(p->stin, STDIN_FILENO) == -1) {
                perror("dup2 stin");
                exit(EXIT_FAILURE);
            }
            close(p->stin);
        }

        if (p->stout != STDOUT_FILENO) {
            if (dup2(p->stout, STDOUT_FILENO) == -1) {
                perror("dup2 stout");
                exit(EXIT_FAILURE);
            }
            close(p->stout);
        }

        if (p->sterr != STDERR_FILENO) {
            if (dup2(p->sterr, STDERR_FILENO) == -1) {
                perror("dup2 sterr");
                exit(EXIT_FAILURE);
            }
            close(p->sterr);
        }

        for (int i = 0; p->fdclose[i] != -1; ++i) {
            close(p->fdclose[i]);
        }

        //printf("Processus fils FDs : STDIN=%d, STDOUT=%d, STDERR=%d\n", p->stin, p->stout, p->sterr);

        // Exécute les commandes non intégrées en utilisant execvp
        if (execvp(p->path, p->argv) == -1) {
            perror("L'exécution de la commande a échoué ou la commande n'a pas été trouvée");
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        // Fork a échoué
        perror("Le fork a échoué");
        exit(EXIT_FAILURE);
    } else {
        // Processus parent

        if (p->stout != STDOUT_FILENO) {
            close(p->stout);
        }

        if (p->stin != STDIN_FILENO) {
            close(p->stin);
        }

        for (int i = 0; p->fdclose[i] != -1; ++i) {
            close(p->fdclose[i]);
        }

        if (!(p->wait)) {
            return status;
        } else {
            waitpid(pid, &status, 0);

            if (WIFEXITED(status)) {
                // Le processus fils s'est terminé normalement
                int exit_status = WEXITSTATUS(status);
                // Vérifier exit_status pour le succès ou l'échec
                if (exit_status == 0) {
                    // Succès
                } else {
                    // Échec
                }
            } else if (WIFSIGNALED(status)) {
                // Le processus fils a été terminé par un signal
                int signal_number = WTERMSIG(status);
                (void)signal_number;
                // Gérer la terminaison du signal
            }

            p->status = status; // Met à jour le champ de statut dans la structure cmd_t
        }
    }

    return status; // Retourne le statut de l'exécution de la commande
}

/**
 * @brief Initialise la structure cmd entré en paramètre
 *
 * @param p La structure à initialiser
 * @return 0 quand terminé
 */
int init_cmd(cmd_t* p) {
    if (p == NULL) {
        return 1; // retourne 1 si le pointeur p est nul
    }

    // Initialise les champs de cmd_t avec des valeurs par défaut
    p->pid = 0;
    p->status = 0;
    p->stin = 0;
    p->stout = 1;
    p->sterr = 2;
    p->wait = 1; // Initialise le processus en arrière-plan par défaut
    p->path = NULL;

    p->next = NULL;
    p->next_success = NULL;
    p->next_failure = NULL;

    // Initialise les tableaux argv et fdclose avec NULL et 0.
    for (int i = 0; i < MAX_CMD_SIZE; ++i) {
        p->argv[i] = NULL;
        p->fdclose[i] = -1;
    }

    return 0; // Retourne 0 comme ré
}

/**
 * @brief Remplit le tableau de commandes en fonction du contenu de tokens.
 *
 * @param tokens le tableau des éléments de la ligne de commandes (peut être modifié)
 * @param cmds le tableau dans lequel sont stockées les différentes structures représentant les commandes.
 * @param max le nombre maximum de commandes dans le tableau
 * @return 0 ou code d'erreur quand terminé
 */
int parse_cmd(char* tokens[], cmd_t* cmds, size_t max) {
    int idx_cmd = 0;
    int idx_tok = 0;
    int idx_argv = 0;

    while (tokens[idx_tok] != NULL) {
        //printf("%s\n", tokens[idx_tok]);
	
	
        if (strcmp(tokens[idx_tok], ";") == 0) {
            cmds[idx_cmd].next = &cmds[idx_cmd + 1];
            idx_cmd++;
            idx_argv = 0;
            idx_tok++;
            continue;
        }

        // Gestion de la redirection d'entrée
        if (strcmp(tokens[idx_tok], "<") == 0) {
            int input_fd = open(tokens[idx_tok + 1], O_RDONLY);
            if (input_fd == -1) {
                perror("Erreur lors de l'ouverture du fichier d'entrée");
                // Gérer l'erreur selon les besoins
                return -1;
            }

            cmds[idx_cmd].stin = input_fd;
            cmds[idx_cmd].argv[idx_argv] = NULL;

            idx_tok += 2;
            continue;
        }

        // Gestion de la redirection de sortie
        if (strcmp(tokens[idx_tok], ">") == 0) {
            cmds[idx_cmd].stout = open(tokens[idx_tok + 1], O_WRONLY | O_CREAT | O_TRUNC, 0640);
            add_fd(cmds[idx_cmd].fdclose, cmds[idx_cmd].stout);
            cmds[idx_cmd].argv[idx_argv] = NULL;
            idx_tok += 2;
            continue;
        }

        // Gestion de la redirection de sortie d'erreur (stderr)
        if (strcmp(tokens[idx_tok], "2>") == 0) {
            cmds[idx_cmd].sterr = open(tokens[idx_tok + 1], O_WRONLY | O_CREAT | O_TRUNC, 0640);
            add_fd(cmds[idx_cmd].fdclose, cmds[idx_cmd].sterr);
            cmds[idx_cmd].argv[idx_argv] = NULL;
            idx_tok += 2;
            continue;
        }

        // Gestion de la redirection de sortie d'erreur - Append (stderr)
        if (strcmp(tokens[idx_tok], "2>>") == 0) {
            cmds[idx_cmd].sterr = open(tokens[idx_tok + 1], O_WRONLY | O_CREAT | O_APPEND, 0640);
            add_fd(cmds[idx_cmd].fdclose, cmds[idx_cmd].sterr);
            cmds[idx_cmd].argv[idx_argv] = NULL;
            idx_tok += 2;
            continue;
        }

        // Gestion de la redirection de sortie ">>"
        if (strcmp(tokens[idx_tok], ">>") == 0) {
            cmds[idx_cmd].stout = open(tokens[idx_tok + 1], O_WRONLY | O_APPEND | O_CREAT, 0640);
            add_fd(cmds[idx_cmd].fdclose, cmds[idx_cmd].stout);
            cmds[idx_cmd].argv[idx_argv] = NULL;
            idx_tok += 2;
            continue;
        }

        // Gestion de l'opérateur & (en arrière-plan)
        if (strcmp(tokens[idx_tok], "&") == 0) {
            cmds[idx_cmd].wait = 0;
            cmds[idx_cmd].argv[idx_argv] = NULL;

            if (tokens[idx_tok] != NULL && strcmp(tokens[idx_tok], "&&") == 0) {
                cmds[idx_cmd].next_success = &cmds[idx_cmd + 1];
            }

            idx_cmd++;
            idx_argv = 0;
            idx_tok++;
            continue;
        }

        // Redirection >&2
        if (strcmp(tokens[idx_tok], ">&2") == 0) {
            cmds[idx_cmd].stout = dup(STDERR_FILENO);
            idx_tok++;
            continue;
        }

        // Redirection 2>&1
        if (strcmp(tokens[idx_tok], "2>&1") == 0) {
            cmds[idx_cmd].sterr = dup(STDOUT_FILENO);
            idx_tok++;
            continue;
        }

        // Gestion de l'opérateur & (en arrière-plan)
        if (strcmp(tokens[idx_tok], "&") == 0) {
            cmds[idx_cmd].wait = 0;
            cmds[idx_cmd].argv[idx_argv] = NULL;

            if (tokens[idx_tok] != NULL && strcmp(tokens[idx_tok], "&&") == 0) {
                cmds[idx_cmd].next_success = &cmds[idx_cmd + 1];
            }

            idx_cmd++;
            idx_argv = 0;
            idx_tok++;
            continue;
        }

        // Gestion de la redirection par pipeline
        if (strcmp(tokens[idx_tok], "|") == 0) {
            int pipe_fd[2];
            if (pipe(pipe_fd) == -1) {
                perror("Erreur lors de la création du pipe");
                return -1;
            }

            cmds[idx_cmd].stout = pipe_fd[1];
            cmds[idx_cmd + 1].stin = pipe_fd[0];

	    add_fd(cmds[idx_cmd + 1].fdclose,pipe_fd[0]); // Ajout des tubes a Fdclose
	    add_fd(cmds[idx_cmd].fdclose,pipe_fd[1]);

            cmds[idx_cmd].argv[idx_argv] = NULL;

            // Configuration explicite des pointeurs suivants pour les commandes dans le pipeline
            cmds[idx_cmd].next = &cmds[idx_cmd + 1];
            cmds[idx_cmd + 1].next = NULL;  // Configuration du suivant de la commande de droite dans le pipeline

            // Passer à la commande suivante dans le tableau
            idx_cmd++;
            idx_argv = 0;
            idx_tok++;
            continue;  // Ne pas incrémenter idx_tok, car le jeton suivant est déjà traité
        }

        // OU logique
        if (tokens[idx_tok] != NULL && strcmp(tokens[idx_tok], "||") == 0) {
            if (tokens[idx_tok + 1] != NULL) {
                cmds[idx_cmd].next_failure = &cmds[idx_cmd + 1];
                idx_cmd++;
            } else {
                // Gérer le cas où il n'y a pas de commande suivante pour le OU logique
                perror("OR ERROR: Pas de commande suivante");
                return -1;
            }
            idx_argv = 0;
            idx_tok++;
            continue;
        }

        // ET logique
        if (tokens[idx_tok] != NULL && strcmp(tokens[idx_tok], "&&") == 0) {
            cmds[idx_cmd].next_success = &cmds[idx_cmd + 1];
            idx_cmd++;
            idx_argv = 0;
            idx_tok++;
            continue;
        }

        if (cmds[idx_cmd].path == NULL) {
            cmds[idx_cmd].path = tokens[idx_tok];
        }

        cmds[idx_cmd].argv[idx_argv] = tokens[idx_tok];
        idx_argv++;
        idx_tok++;
    }
    return 0;  // Retourne 0 ou gère les codes d'erreur en conséquence
}

void add_fd(int *fdclose, int fd) {
    int i = 0;
    while (fdclose[i] != -1 && fdclose[i] != 0) {
        i++;
    }
    fdclose[i] = fd;
}

void merge_fdclose(int *dest, int *src) {
    int i = 0;
    while (src[i] != -1 && src[i] != 0) {
        add_fd(dest, src[i]);
        i++;
    }
}

