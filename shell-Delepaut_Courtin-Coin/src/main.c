/*
  Projet minishell - Licence 3 Info - PSI 2023
 
  Nom :
  Prénom :
  Num. étudiant :
  Groupe de projet :
  Date :
 
  Interface du minishell.
 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "parser.h"
#include "cmd.h"
//#include "builtin.h"

int main(int argc, char* argv[]) {
  char cmdline[MAX_LINE_SIZE]; // buffer des lignes de commandes
  char* TMPCMDLINE = NULL;
  char* cmdtoks[MAX_CMD_SIZE]; // "mots" de la ligne de commandes
  cmd_t cmds[MAX_CMD_SIZE];
  cmd_t* current;

  //Chemin fichier
  char cwd[MAX_LINE_SIZE];
  char* home_cwd = NULL;

  //initialise tous les tableau de pointeurs à NULL
  for (int i=0; i<MAX_CMD_SIZE; i++) {
    cmdtoks[i]=NULL;
    cmds[i].path=NULL;
    for(int j=0; j<MAX_CMD_SIZE; j++)
      cmds[i].argv[j]=NULL;
  }

  //for (int i = 0; i < MAX_CMD_SIZE; i++) {init_cmd(&cmds[i]);}
  init_cmd(&cmds[0]);

  //Récupère les variables pour HOME / USER / COMPUTER
  char* home = getenv("HOME");
  if (home == NULL) {
    home = "C:\\Users\\delep";  // Provide a default value
  }
  char* user = getenv("USER");
  if (user == NULL) {
    // Handle the case where the "USER" environment variable is not set
    user = "unknown"; // Provide a default value
}

  while (1) {
    printf("Debut\n");

    // Effacer les contenus de cmdline, cmdtoks et cmds
    memset(cmdline, 0, sizeof(cmdline));
    for(int i=0;i<MAX_CMD_SIZE;i++) {
      if(cmdtoks[i]!=NULL){
        free(cmdtoks[i]);
        cmdtoks[i] = NULL;
      }
    }

    // Initialiser les valeurs par défaut dans cmds (stdin, stdout, stderr, ...)
    

    //Initialise home_cwd
    getcwd(cwd, MAX_LINE_SIZE-1);
    size_t len = strlen(home) + strlen(cwd) + 1;
    home_cwd = (char*)malloc(len);
    if (home_cwd != NULL) {
      strcpy(home_cwd, home);
      strcat(home_cwd, "/");
      strcat(home_cwd, cwd + strlen(home));
    } else {
      printf("Memory allocation failed\n");
    }

    // Afficher un prompt et affiche [USER] [COMPUTER] [ENV] [DIRECTORY] et les met en couleur
    printf("\n\033[32m%s\033[0m \033[35mMINISHELL-WIN\033[0m \033[33m~%s\033[0m",user,home_cwd);
    printf("\n");
    printf("$ ");


    // Lire une ligne dans cmdline - Attention fgets enregistre le \n final
    if (fgets(cmdline, MAX_LINE_SIZE, stdin)==NULL) {
      break;    //libere home_cwd
    }
    cmdline[strlen(cmdline)-1]='\0';
    if(TMPCMDLINE!=NULL){free(TMPCMDLINE);}
    TMPCMDLINE = strdup(cmdline);

    //Affiche la chaine original
    printf("Original:\"%s\"\n",TMPCMDLINE);

    // Traiter la ligne de commande
    //   - supprimer les espaces en début et en fin de ligne
    trim(TMPCMDLINE);
    printf("Trim:\"%s\"\n",TMPCMDLINE);

    //   - ajouter d'éventuels espaces autour de ; ! || && & ...
    separate_s(TMPCMDLINE,"<&;|&\\\">2",MAX_CMD_SIZE);
    printf("Separate:\"%s\"\n",TMPCMDLINE);

    //   - supprimer les doublons d'espaces
    clean(TMPCMDLINE);
    printf("clean:\"%s\"\n",TMPCMDLINE);

    //   - traiter les variables d'environnement
    substenv(TMPCMDLINE,MAX_CMD_SIZE);

    // Découper la ligne dans cmdtoks
    strcut(TMPCMDLINE,' ',cmdtoks,MAX_CMD_SIZE);
    for (int i=0; i<MAX_CMD_SIZE; i++) {
      if (cmdtoks[i] != NULL && cmdtoks[i] != '\0' ){
        printf("tokens[%d]:\"%s\"\n",i, cmdtoks[i]);
      }
    }
    //substenv(TMPCMDLINE,MAX_LINE_SIZE);
    //printf("sub:\"%s\"\n",TMPCMDLINE);



    /*  
    // Découper la ligne dans cmdtoks
    strcut(TMPCMDLINE,' ',cmdtoks,MAX_CMD_SIZE);
    for (int i=0; i<MAX_CMD_SIZE; i++) if (cmdtoks[i] != NULL && cmdtoks[i] != '\0' )printf("tokens[%d]:\"%s\"\n",i, cmdtoks[i]);
    */
    
    continue;

    // Traduire la ligne en structures cmd_t dans cmds
    
    // Les commandes sont chaînées en fonction des séparateurs
    //   - next -> exécution inconditionnelle
    //   - next_success -> exécution si la commande précédente réussit
    //   - next_failure -> exécution si la commande précédente échoue

    // Exécuter les commandes dans l'ordre en fonction des opérateurs
    // de flux
    for (current=cmds; current!=NULL; ) {
      
      // Lancer la commande
      //exec_cmd(current);
      
    }
  }
  fprintf(stderr, "\nGood bye!\n");
  return 0;
}
