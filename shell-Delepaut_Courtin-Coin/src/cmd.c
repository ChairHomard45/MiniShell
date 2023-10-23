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
#include <stddef.h>

int exec_cmd(cmd_t* p) {
 return 0;
}
/**
 * @brief Initialise la structure cmd entré en paramètre
 * 
 * @param p La structure a initialiser
 * @return 0 quand terminer
 */
int init_cmd(cmd_t* p) {
  p->pid = -1; //PID Valeur Défaut
  p->status = 0;
  if (p->stin>2)
    close(p->stin); //fermer si fichier autre que stdin stdout stderr
  p->stin = 0; //valeur par défaut (stdin)
  if (p->stout>2)
    close(p->stout); //fermer si fichier autre que stdin stdout stderr
  p->stout = 1; //valeur par défaut (stdout)
  if (p->sterr>2)
    close(p->sterr); //fermer si fichier autre que stdin stdout stderr
  p->sterr = 2; //valeur par défaut (stderr)
  p->wait = 1; //wait Valeur Défaut
  
  /*for (int j=0; j<MAX_CMD_SIZE; j++) {
    if (p->fdclose[j] != -1){
      close(p->fdclose[j]);
      p->fdclose[j] = -1;
    }
  }*/
  
  //coupe la chaine d'exécution
  p->next=NULL;
  p->next_success=NULL;
  p->next_failure=NULL;
  return 0;
}

int parse_cmd(char* tokens[], cmd_t* cmds, size_t max) {
  return 0;
}
