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
    
    if(p->path == NULL || p->argv == NULL){
    	if (write(STDERR_FILENO, "Erreur: path et argv NULL", strlen("Erreur: path et argv NULL")) == -1) {
            perror("Erreur ecriture to fderr");
            // Handle error as needed
            exit(EXIT_FAILURE);
        }
    	return -1;
    }
    

        pid_t pid = fork();
        
        if (pid == 0) {
            // Child process
		
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
		
                // Execute non-built-in commands using execvp
                if (execvp(p->path, p->argv) == -1) {
                    perror("Command execution failed or Command not found");
                    exit(EXIT_FAILURE);
                }

        } else if (pid < 0) {
            // Fork failed
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else {
            // Parent process
            if(!(p->wait)){
				return status;
			}else{
			waitpid(pid, &status, 0);
			}
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
    p->wait = 1; //initialise le bg processus par default
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
    int idx_cmd = 0;
    int idx_tok = 0;
    int idx_argv = 0;
    
    while (tokens[idx_tok] != NULL){
    	
    	if(strcmp(tokens[idx_tok], ";") == 0){
    		cmds[idx_cmd].next = &cmds[idx_cmd+1];
    		
    		idx_cmd++;
    		idx_argv = 0;
    		idx_tok++;	
    		continue;
    	}
    	
    	// Input Redirection
        if (strcmp(tokens[idx_tok], "<") == 0) {
   		int input_fd = open(tokens[idx_tok + 1], O_RDONLY);
    		if (input_fd == -1) {
        		perror("Error opening input file");
        		// Handle error as needed
        		return -1;  // or another error code
    		}
    
    	cmds[idx_cmd].stin = input_fd;
    	cmds[idx_cmd].argv[idx_argv] = NULL;

    	idx_tok += 2;
    	continue;
	}

        // Output Redirection
        if (strcmp(tokens[idx_tok], ">") == 0) {
            cmds[idx_cmd].stout = open(tokens[idx_tok + 1], O_WRONLY | O_CREAT | O_TRUNC, 0640);
            add_val(cmds[idx_cmd].fdclose, cmds[idx_cmd].stout);
            cmds[idx_cmd].argv[idx_argv] = NULL;
            idx_tok += 2;
            continue;
        }

        // Error Output Redirection (stderr)
        if (strcmp(tokens[idx_tok], "2>") == 0) {
            cmds[idx_cmd].sterr = open(tokens[idx_tok + 1], O_WRONLY | O_CREAT | O_TRUNC, 0640);
            
            add_val(cmds[idx_cmd].fdclose, cmds[idx_cmd].sterr);
            
            cmds[idx_cmd].argv[idx_argv] = NULL;
            
            idx_tok += 2;
            
            continue;
        }

        // Error Output Redirection - Append (stderr)
        if (strcmp(tokens[idx_tok], "2>>") == 0) {
            cmds[idx_cmd].sterr = open(tokens[idx_tok + 1], O_WRONLY | O_CREAT | O_APPEND, 0640);
            
            add_val(cmds[idx_cmd].fdclose, cmds[idx_cmd].sterr);
            
            cmds[idx_cmd].argv[idx_argv] = NULL;
            
            idx_tok += 2;
            continue;
        }
    	
    	
    	// Verifier la redirection de sortie ">>"
    	if(strcmp(tokens[idx_tok], ">>") == 0){
    		cmds[idx_cmd].stout = open(tokens[idx_tok+1],O_WRONLY | O_APPEND | O_CREAT,0640);
    		
    		add_val(cmds[idx_cmd].fdclose,cmds[idx_cmd].stout);
    		
    		cmds[idx_cmd].argv[idx_argv] = NULL;
    		
    		idx_tok += 2;
    		continue;
    	}
    	
    	
    	// Vérifier l'Operateur & (en arriere plan)
    	if (strcmp(tokens[idx_tok],"&")==0){
    		cmds[idx_cmd].wait = 0;
    		
    		cmds[idx_cmd].argv[idx_argv] = NULL;
    		
    		if(tokens[idx_tok] != NULL && strcmp(tokens[idx_tok], "&&")==0){
    			cmds[idx_cmd].next_success = &cmds[idx_cmd+1];
    		}
    		idx_cmd++;
    		idx_argv = 0;
    		idx_tok++;
    		continue;
    	}
    	
    	// Pipe Redirection
		
    	if (strcmp(tokens[idx_tok], "|") == 0) {
    		int pipe_fd[2];

    		if (pipe(pipe_fd) == -1) {
        		perror("Error creating pipe");
        		return -1;  // or another error code
    		}

    		cmds[idx_cmd].stout = pipe_fd[1];
    		cmds[idx_cmd + 1].stin = pipe_fd[0];

    		// Add file descriptors to close for each command
    		add_fd(cmds[idx_cmd].fdclose, pipe_fd[0]);
    		add_fd(cmds[idx_cmd + 1].fdclose, pipe_fd[1]);

    		cmds[idx_cmd].argv[idx_argv] = NULL;

    		// Explicitly set up next pointers for the commands in the pipeline
    		cmds[idx_cmd].next = &cmds[idx_cmd + 1];
    		cmds[idx_cmd + 1].next = NULL;  // Set the next of the right command in the pipeline

    		// Move to the next command in the array
    		idx_cmd += 2;
    		idx_argv = 0;
    		idx_tok++;
    		continue;  // Skip incrementing idx_tok, as the next token is already processed
		}


        // Logical OR
        if (tokens[idx_tok] != NULL && strcmp(tokens[idx_tok], "||") == 0) {
    		if (tokens[idx_tok + 1] != NULL) {
        		cmds[idx_cmd].next_failure = &cmds[idx_cmd + 1];
    		} else {
        		// Handle the case where there's no next command for logical OR
        		perror("OR ERROR: No next command");
        		exit(EXIT_FAILURE);
    		}
    		cmds[idx_cmd].argv[idx_argv] = NULL;
    		idx_cmd++;
    		idx_argv = 0;
    		idx_tok++;
    		continue;
		}
    	
    	if (tokens[idx_tok] != NULL && strcmp(tokens[idx_tok], "&&") == 0) {
    		cmds[idx_cmd].next_success = &cmds[idx_cmd + 1];
    		cmds[idx_cmd].argv[idx_argv] = NULL;
    		idx_cmd++;

    		// Check if the next command is available
    		if (cmds[idx_cmd].path != NULL) {
       	 		idx_argv = 0;
        		idx_tok++;
        		continue;
    		} else {
        	// No next command, execute the next_success command
        		if (cmds[idx_cmd].next_success != NULL) {
            		idx_cmd = cmds[idx_cmd].next_success - cmds;
            		continue;
        		} else {
            		// Handle the case where there's no next_success command
            		break;
        		}
    		}
	}
    	
    	if(cmds[idx_cmd].path == NULL){
    		cmds[idx_cmd].path = tokens[idx_tok];
    	}
    	
    	cmds[idx_cmd].argv[idx_argv] = tokens[idx_tok];
    	idx_argv++;
    	idx_tok++;
    	
	}
    return 0;  // Return 0 or handle error codes accordingly
}

void add_val(int *fdclose, int val) {
    int i = 0;
    while (fdclose[i] != -1 && fdclose[i] != 0) {
        i++;
    }
    fdclose[i] = val;
}


void add_fd(int *fdclose,int fd){
	int i = 0;
	while(fdclose[i] != -1 && fdclose[i] !=0){
		i++;
	}
	fdclose[i] = fd;
}
	
void merge_fdclose(int *dest,int *src){
	int i = 0;
	while(src[i] != -1 && src[i] != 0){
		add_fd(dest,src[i]);
		i++;
	}	
}

