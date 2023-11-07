/*
  Projet minishell - Licence 3 Info - PSI 2023
 
  Nom :
  Prénom :
  Num. étudiant :
  Groupe de projet :
  Date :
 
  Gestion des commandes internes du minishell (implémentation).
 
 */


#include "cmd.h"
#include "builtin.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int is_builtin(const char* cmd) {
  if (strcmp(cmd, "cd") == 0 || strcmp(cmd, "exit") == 0 || strcmp(cmd, "export") == 0 || strcmp(cmd, "echo") == 0 || strcmp(cmd, "unset") == 0) {
        return 1; // Command is a built-in command
    }
    return 0; // Command is not a built-in command
}

int builtin(cmd_t* cmd) {
  if (strcmp(cmd->path, "cd") == 0) {
        return cd(cmd->argv[0], cmd->sterr);
    } else if (strcmp(cmd->path, "exit") == 0) {
        return exit_shell(0, cmd->stout);
    }else if (strcmp(cmd->path, "export") == 0) {
        return export(cmd->argv[0],cmd->argv[1],cmd->sterr);
    }else if (strcmp(cmd->path, "echo") == 0) {
       return echo(cmd);
    }else if (strcmp(cmd->path, "unset") == 0) {
       return unset(cmd->argv[0]);
    }
    return -1; // Unrecognized command
}

int cd(const char* path, int fderr) {
    if(path == NULL){
	write(fderr, "Error changement de répertoir: Path NULL",strlen("Error changement de répertoire: Path NULL"));
        return -1;
    }
    if (chdir(path)==-1) {
        write(fderr, "Error changement de répertoire",strlen("Error changement de répertoire"));
        return -1;
    }
    return 0;
}

int export(const char* var, const char* value, int fderr) {
  if(!var||!value){
  	write(fderr,"Error pas assez d'argument",strlen("Error pas assez d'argument"));
  	return 1;
  }
  if (setenv(var, value, 1) == -1) {
  	write(fderr, "Error: Could not set environment variable", strlen("Error: Could not set environment variable"));
  	return -1;
  }
    return 0;
}


/**
 * @brief affiche la chaine ou le contenu du fichier ou le retour d'execution
 * 
 * @param p processus contenant les arguments nécessaires
 * @return 0 si succes, 1 sinon
 */
int echo(cmd_t * p) {
  FILE* output = stdout; // Default output to stdout	
  FILE* input = stdin; // Default input from stdin

  if (!strcmp(p->argv[0], "-s")) { // String input
        if (!p->argv[1]) {
            fprintf(stderr, "Insufficient arguments\n");
            return 1;
        }
        int j = 1;
        while (p->argv[j] != NULL) {
            printf("%s ", p->argv[j]); // Display the string
            j++;
        }
        printf("\n");
        return 0;
    }
    else if (!strcmp(p->argv[0], "-f")) { // File input
        if (!p->argv[1]) {
            fprintf(stderr, "Insufficient arguments\n");
            return 1;
        }
        char print[MAX_LINE_SIZE];
        FILE* file = fopen(p->argv[1], "r"); // Open the file for reading
        if (!file) {
            fprintf(stderr, "File not found\n");
            return 1;
        }
        while (fgets(print, MAX_LINE_SIZE, file)) {
            printf("%s", print); // Display each line in the file
        }
        printf("\n");
        fclose(file); // Close the file
        return 0;
    }else {
        // Check for output redirection operators
        for (int i = 1; p->argv[i] != NULL; i++) {
            if (strcmp(p->argv[i], ">") == 0) { // Output redirection (>)
                if (p->argv[i + 1] != NULL) {
                    output = fopen(p->argv[i + 1], "w");
                    if (!output) {
                        fprintf(stderr, "Error opening file for writing\n");
                        return 1;
                    }
                }
                else {
                    fprintf(stderr, "Insufficient arguments for output redirection\n");
                    return 1;
                }
            }
            else if (strcmp(p->argv[i], ">>") == 0) { // Output redirection (>>)
                if (p->argv[i + 1] != NULL) {
                    output = fopen(p->argv[i + 1], "a");
                    if (!output) {
                        fprintf(stderr, "Error opening file for writing\n");
                        return 1;
                    }
                }
                else {
                    fprintf(stderr, "Insufficient arguments for output redirection\n");
                    return 1;
                }
            }
            else if (strcmp(p->argv[i], "<") == 0) { // Input redirection (<)
                if (p->argv[i + 1] != NULL) {
                    input = fopen(p->argv[i + 1], "r");
                    if (!input) {
                        fprintf(stderr, "Error opening file for reading\n");
                        return 1;
                    }
                }
                else {
                    fprintf(stderr, "Insufficient arguments for input redirection\n");
                    return 1;
                }
            }else if (strcmp(p->argv[i], "<<") == 0) { // Here-document redirection (<<)
                if (p->argv[i + 1] != NULL) {
                    // Read input until the delimiter
                    char delimiter[MAX_LINE_SIZE];
                    strcpy(delimiter, p->argv[i + 1]);

                    char input_line[MAX_LINE_SIZE];
                    while (fgets(input_line, MAX_LINE_SIZE, stdin) != NULL) {
                        // Check if the line matches the delimiter
                        if (strcmp(input_line, delimiter) == 0) {
                            break; // Exit the loop if delimiter is found
                        }
                        fprintf(output, "%s", input_line); // Output the line
                    }
                }
                else {
                    fprintf(stderr, "Insufficient arguments for here-document redirection\n");
                    return 1;
                }
            
        }
    }
    }
    if (output != stdout) {
        fclose(output); // Close the file if output was redirected
    }
    
    if (input != stdin) {
        fclose(input); // Close the file if input was redirected
    }
    
    printf("\n");
    return 0;
}

int unset(const char* var){
	unsetenv(var);
}

int exit_shell(int ret, int fderr) {
  write(fderr,"Goodbye!",strlen("Goodbye!"));
  exit(ret);
}
