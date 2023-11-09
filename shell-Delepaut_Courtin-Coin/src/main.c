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
#include "builtin.h"

int main(int argc, char* argv[]) {
  char cmdline[MAX_LINE_SIZE]; // buffer des lignes de commandes
  char* TMPCMDLINE = NULL;
  char* cmdtoks[MAX_CMD_SIZE]; // "mots" de la ligne de commandes
  cmd_t cmds[MAX_CMD_SIZE];
  cmd_t* current;

  //Chemin fichier
  char cwd[MAX_LINE_SIZE];

  //initialise tous les tableau de pointeurs à NULL
  for (int i=0; i<MAX_CMD_SIZE; i++) {
    cmdtoks[i]=NULL;
    cmds[i].path=NULL;
    for(int j=0; j<MAX_CMD_SIZE; j++)
      cmds[i].argv[j]=NULL;
  }
  //Récupère les variables pour USER 
  char* user = getenv("USER");
  if (user == NULL) {
    // Handle the case where the "USER" environment variable is not set
    user = "unknown"; // Provide a default value
}

  int result2;

  while (1) {
    printf("\nDebut\n");

    // Effacer les contenus de cmdline, cmdtoks et cmds
    cmdline[0] = '\0';
    for(int i=0;i<MAX_CMD_SIZE;i++) {
      if(cmdtoks[i]!=NULL){
        cmdtoks[i] = '\0';
      }
    }

    // Initialiser les valeurs par défaut dans cmds (stdin, stdout, stderr, ...)
    for (int i = 0; i < MAX_CMD_SIZE; i++) {init_cmd(&cmds[i]);}
    current=NULL;

    //Initialise home_cwd
    getcwd(cwd, MAX_LINE_SIZE-1);
    
    // Afficher un prompt et affiche [USER] [COMPUTER] [ENV] [DIRECTORY] et les met en couleur
    printf("\n\033[32m%s\033[0m \033[35mMINISHELL-WIN\033[0m \033[33m~%s\033[0m",user,cwd);
    printf("\n");
    printf("$ ");


    // Lire une ligne dans cmdline - Attention fgets enregistre le \n final
    if (fgets(cmdline, MAX_LINE_SIZE, stdin)==NULL) {
      continue;    
    }
    cmdline[strlen(cmdline)-1]='\0';
    // Deallocation of allocated memory in the loop
    if (TMPCMDLINE != NULL) {
         free(TMPCMDLINE);
         TMPCMDLINE = NULL; // Reset to avoid dangling pointer
    }
    TMPCMDLINE = strdup(cmdline);

    //Affiche la chaine original
    printf("Original:\"%s\"\n",TMPCMDLINE);

    // Traiter la ligne de commande
    //   - supprimer les espaces en début et en fin de ligne
    if(trim(TMPCMDLINE)<=0){printf("Trim Fail");continue;}
    printf("Trim:\"%s\"\n",TMPCMDLINE);
    
    //   - ajouter d'éventuels espaces autour de ; ! || && & ...
    if(separate_s(TMPCMDLINE,";|&<>!",MAX_CMD_SIZE)==0)continue;
    printf("Separate:\"%s\"\n",TMPCMDLINE);

    //   - supprimer les doublons d'espaces
    if(clean(TMPCMDLINE)<=0){
    	printf("Clean Fail");
    	continue;
    }
    printf("clean:\"%s\"\n",TMPCMDLINE);
 
 
    //   - traiter les variables d'environnement
    if(substenv(TMPCMDLINE,MAX_CMD_SIZE)<0){
    	printf("Substenv Fail");
    	continue;
    }
    printf("Substenv:\"%s\"\n",TMPCMDLINE);
    	
    // Découper la ligne dans cmdtoks
    int n;
    if( (n=strcut(TMPCMDLINE,' ',cmdtoks,MAX_CMD_SIZE))<=0){
    	printf("strcut Fail");
    	continue;
    }
    
  	for (int i=0; i<MAX_CMD_SIZE; i++) {
     		if (cmdtoks[i] != NULL ){
        		printf("tokens[%d]:\"%s\"\n",i, cmdtoks[i]);
     		}
  	}
    

    // Traduire la ligne en structures cmd_t dans cmds
    if(parse_cmd(cmdtoks,cmds,n)!=0){
    	printf("Parse Fail");
    	continue;
    }

    // Les commandes sont chaînées en fonction des séparateurs
    //   - next -> exécution inconditionnelle
    //   - next_success -> exécution si la commande précédente réussit
    //   - next_failure -> exécution si la commande précédente échoue

    // Exécuter les commandes dans l'ordre en fonction des opérateurs
    // de flux
    current = cmds;  // Start with the first command
    while (current != NULL) {
    // Launch the command
    if(current->path == NULL){
    	break;
    } else if (is_builtin(current->path)) {
        result2 = builtin(current);
    } else {
        result2 = exec_cmd(current);
    }

    // Check the result of the command execution
    if (result2 != 0) {
        break;  // Exit the loop if the command execution fails
    }
    
    // Move to the next command structure
	if (current->next_success != NULL && current->status == 0) {
    		current = current->next_success;
    		printf("here1: %s\n", current->path);
	} else if (current->next_failure != NULL && current->status != 0) {
    		current = current->next_failure;
    		printf("here2: %s\n", current->path);
	} else if (current->next != NULL) {
    		current = current->next;
        	printf("here3: current is not NULL\n");
	} else {
    		printf("here4: current is NULL\n");
    		break; // No more commands, exit the loop
	}
     }
  }
  fprintf(stderr, "\nGood bye!\n");
  return 0;
}
