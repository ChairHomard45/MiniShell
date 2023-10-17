/*
  Projet minishell - Licence 3 Info - PSI 2023

  Nom : Delepaut Courtin-coin
  Prénom : Clement Theo
  Num. étudiant : 22108706
  Groupe de projet : 15
  Date :
 
  Gestion des processus (implémentation).
 
 */

#include "cmd.h"

int exec_cmd(cmd_t* p) {

}

int init_cmd(cmd_t* p) {
  p->pid = -1; //PID Valeur Défaut
  p->status = 0;
  p->stdin = 0; //stdin Valeur Défaut
  p->stdout = 1; //stdout Valeur Défaut
  p->stderr = 2; //stderr Valeur Défaut
  p->wait = 1; //wait Valeur Défaut
  for (int j=0; j<MAX_CMD_SIZE; j++) {
    if (p->fdclose[j] != -1){
      close(p->fdclose[j]);
      p->fdclose[j] = -1;
    }
  }
  p->next=NULL;
  p->next_success=NULL;
  p->next_failure=NULL;
  return 0;
}

int parse_cmd(char* tokens[], cmd_t* cmds, size_t max) {
  
}
