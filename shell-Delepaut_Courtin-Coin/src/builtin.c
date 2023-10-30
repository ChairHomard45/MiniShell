/*
  Projet minishell - Licence 3 Info - PSI 2023
 
  Nom :
  Prénom :
  Num. étudiant :
  Groupe de projet :
  Date :
 
  Gestion des commandes internes du minishell (implémentation).
 
 */

int is_builtin(const char* cmd) {
  if (strcmp(cmd, "cd") == 0 || strcmp(cmd, "exit") == 0 || strcmp(cmd, "export") == 0 || strcmp(cmd, "echo") == 0) {
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
    }
    return -1; // Unrecognized command
}

int cd(const char* path, int fderr) {
  int ret = chdir(path);
  printf("path:%s",path);
    if (ret == -1) {
        dprintf(fderr, "Error: Could not change directory to %s\n", path);
    }
    return ret;
}

int export(const char* var, const char* value, int fderr) {
  if(!var||!value){
  	dprintf(fderr,"Error: not enough arguments:%s and %s",var,value);
  	return 1;
  }
  int ret = setenv(var, value, 1);
    if (ret == -1) {
        dprintf(fderr, "Error: Could not set environment variable %s to %s\n", var, value);
    }
    return ret;
}

/**
 * @brief affiche la chaine ou le contenu du fichier ou le retour d'execution
 * 
 * @param p processus contenant les arguments nécessaires
 * @return 0 si succes, 1 sinon
 */
int echo(cmd_t * p) {
  if (!strcmp(p->argv[0], "-s")) { //string
    if (!p->argv[1]) {
      write(p->sterr, "Arguments insuffisants\n", 24); 
      return 1;
    }
    printf("%s\n", p->argv[1]); //affiche la chaine
    return 0;
  }
  if (!strcmp(p->argv[0], "-f")) { //file
    if (!p->argv[1]) {
      write(p->sterr, "Arguments insuffisants\n", 24);
      return 1;
    }
    char print[MAX_LINE_SIZE];
    FILE * file = fopen(p->argv[1], "r"); //ouvre le fichier
    if (!file) { //fichier introuvable
      write(p->sterr, "Le fichier n'a pas été trouvé\n", 34);
      return 1;
    }
    while (fgets(print, MAX_LINE_SIZE, file)) {
      printf("%s", print); //affiche la ligne
    }
    printf("\n");
    return 0;
  }
  printf("\n");
  return 0;
}


int exit_shell(int ret, int fderr) {
  dprintf(fderr, "Exiting the shell with code %d\n", ret);
    exit(ret);
}
