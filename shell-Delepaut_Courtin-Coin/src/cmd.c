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
 * @brief remplis le tableau de commandes en fonction du contenu de tokens.
 *
 * @param tokens le tableau des éléments de la ligne de commandes (peut être modifié)
 * @param cmds le tableau dans lequel sont stockés les différentes structures représentant le commandes.
 * @param max le nombre maximum de commandes dans le tableau
 * @return 0 ou code d'erreur quand terminer
 */
int parse_cmd(char* tokens[], cmd_t* cmds, size_t max) {
    int cmd_index = 0;
    int token_index = 0;


    // Iterate through the tokens and create cmd_t structures
    while (tokens[token_index] != NULL) {
        cmd_t* current_cmd = &cmds[cmd_index];

        // Initialize the cmd_t structure's fields
        current_cmd->pid = 0; // Initialize as needed
        current_cmd->status = 0; // Initialize as needed
        current_cmd->stin = 0; // Initialize as needed
        current_cmd->stout = 1; // Initialize as needed
        current_cmd->sterr = 2; // Initialize as needed
        current_cmd->wait = 0; // Initialize as needed

        current_cmd->path = tokens[token_index];
        token_index++;

        int argv_index = 0;
        while (tokens[token_index] != NULL && strcmp(tokens[token_index], "|") != 0) {
            current_cmd->argv[argv_index] = tokens[token_index];
            token_index++;
            argv_index++;
        }
        current_cmd->argv[argv_index] = NULL;

         // Check for input/output redirection symbols
            if (tokens[token_index] != NULL) {
                if (strcmp(tokens[token_index], ">") == 0) {
                    token_index++;
                    // Process output redirection
                    if (tokens[token_index] != NULL && strcmp(tokens[token_index], ">") == 0) {
                        // Handle ">>" for appending
                        token_index++;
                        current_cmd->stout = open(tokens[token_index], O_WRONLY | O_CREAT | O_APPEND, 0644);
                    } else {
                        // Regular output redirection ">"
                        current_cmd->stout = open(tokens[token_index], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    }
                    token_index++;
                } else if (strcmp(tokens[token_index], "<") == 0) {
                    token_index++;
                    // Process input redirection
                    if (tokens[token_index] != NULL && strcmp(tokens[token_index], "<") == 0) {
                        // Handle "<<" for a here document
                        token_index++;
                        // Handle here document (implementation-specific)
                    } else {
                        // Regular input redirection "<"
                        current_cmd->stin = open(tokens[token_index], O_RDONLY);
                    }
                    token_index++;
                } else if (strcmp(tokens[token_index], "2>") == 0) {
                    token_index++;
                    // Process standard error redirection
                    current_cmd->sterr = open(tokens[token_index], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    token_index++;
                } else if (strcmp(tokens[token_index], "2>>") == 0) {
                    token_index++;
                    // Process appending standard error
                    current_cmd->sterr = open(tokens[token_index], O_WRONLY | O_CREAT | O_APPEND, 0644);
                    token_index++;
                }
            }

        // Handle pipe "|" if present and create linkages accordingly
        if (tokens[token_index] != NULL && strcmp(tokens[token_index], "|") == 0) {
            token_index++;
            current_cmd->next = &cmds[cmd_index + 1];
        } else {
            current_cmd->next = NULL;
        }

        // Increment cmd_index for the next command
        cmd_index++;
    }

    return 0; // Return 0 or handle error codes accordingly
}
