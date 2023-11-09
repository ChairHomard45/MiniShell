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
  if (strcmp(cmd, "cd") == 0 || strcmp(cmd, "exit") == 0 || strcmp(cmd, "export") == 0 || strcmp(cmd, "unset") == 0) {
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
    }else if (strcmp(cmd->path, "unset") == 0) {
       return unset(cmd->argv[1]);
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

int unset(const char* var){
	unsetenv(var);
}


int exit_shell(int ret, int fderr) {
  write(fderr,"Goodbye!\n",strlen("Goodbye!\n"));
  exit(ret);
}
