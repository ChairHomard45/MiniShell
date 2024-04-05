/*
  Projet minishell - Licence 3 Info - PSI 2023

  Nom : Delepaut Courtin-Coin
  Prénom : Clement Théo
  Num. étudiant : 22108706 22100975
  Groupe de projet : 15
  Date : 24 / 11 / 2023

  Fichier main.h: Interface du minishell.
  Dependances: parser.h cmd.h builtin.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "parser.h"
#include "cmd.h"
#include "builtin.h"


int main(int argc, char* argv[]) {
    // Déclaration des variables
    char cmdline[MAX_LINE_SIZE]; // buffer des lignes de commandes
    char* TMPCMDLINE = NULL;
    char* cmdtoks[MAX_CMD_SIZE]; // "mots" de la ligne de commandes
    cmd_t cmds[MAX_CMD_SIZE];
    cmd_t* current;

    // Chemin fichier
    char cwd[MAX_LINE_SIZE];
    
    // Initialise tous les tableaux de pointeurs à NULL
    for (int i = 0; i < MAX_CMD_SIZE; i++) {
        cmdtoks[i] = NULL;
        cmds[i].path = NULL;
        for (int j = 0; j < MAX_CMD_SIZE; j++)
            cmds[i].argv[j] = NULL;
    }

    // Récupère les variables pour USER
    char* user = getenv("USER");
    if (user == NULL) {
        // Gérer le cas où la variable d'environnement "USER" n'est pas définie
        user = "unknown"; // Fournir une valeur par défaut
    }

    

    while (1) {
        //printf("\nDebut\n");

        // Effacer les contenus de cmdline, cmdtoks et cmds
        cmdline[0] = '\0';
        for (int i = 0; i < MAX_CMD_SIZE; i++) {
            cmdtoks[i] = NULL;  // Set each element to NULL to clear cmdtoks
        }

        // Initialiser les valeurs par défaut dans cmds (stdin, stdout, stderr, ...)
        for (int i = 0; i < MAX_CMD_SIZE; i++) {
            init_cmd(&cmds[i]);
        }
        current = NULL;

        // Initialise home_cwd
        if (getcwd(cwd, MAX_LINE_SIZE - 1) == NULL) {
            fprintf(stderr, "Error: PWD environment variable is not set.\n");
            // Handle the error as needed, for example, exit the program
            exit(EXIT_FAILURE);
        }

        // Afficher un prompt personnalisé
        printf("\033[1m[PID = %d]\033[0m \033[1m\033[32m%s@MINISHELL_WIN-LINU:\033[0m \033[1m\033[34m%s\033[0m$ ", getpid(), user, cwd);

        // Lire une ligne dans cmdline - Attention fgets enregistre le \n final
        if (fgets(cmdline, MAX_LINE_SIZE, stdin) == NULL) {
            continue;
        }
        cmdline[strlen(cmdline) - 1] = '\0';

        // Gestion de la commande "history"
        if (strcmp(cmdline, "history") == 0) {
            print_history();
            continue;
        } else if (strcmp(cmdline,"HELP") == 0) {
            print_command_description(cmdline);
            continue;
        }
	
	execute_previous_command(cmdline);
	
        // Ajouter la commande à l'historique
        add_to_history(cmdline);

        // Deallocation de la mémoire allouée dans la boucle
        if (TMPCMDLINE != NULL) {
            free(TMPCMDLINE);
            TMPCMDLINE = NULL; // Réinitialiser pour éviter un pointeur errant
        }
        TMPCMDLINE = strdup(cmdline);

        // Affiche la chaîne originale
        //printf("Original:\"%s\"\n", TMPCMDLINE);

        // Traiter la ligne de commande
        //   - supprimer les espaces en début et en fin de ligne
        if (trim(TMPCMDLINE) <= 0) {
            //printf("Trim Fail");
            continue;
        }
        //printf("Trim:\"%s\"\n", TMPCMDLINE);

        //   - ajouter d'éventuels espaces autour de ; ! || && & ...
        if (separate_s(TMPCMDLINE, "|&<>!;2", MAX_CMD_SIZE) != 0){
            //printf("Separate Fail");
            continue;
        }
        //printf("Separate:\"%s\"\n", TMPCMDLINE);

        //   - supprimer les doublons d'espaces
        if (clean(TMPCMDLINE) <= 0) {
            //printf("Clean Fail");
            continue;
        }
        //printf("clean:\"%s\"\n", TMPCMDLINE);

        //   - traiter les variables d'environnement
        if (substenv(TMPCMDLINE, MAX_CMD_SIZE) < 0) {
            //printf("Substenv Fail");
            continue;
        }
        //printf("Substenv:\"%s\"\n", TMPCMDLINE);

        // Découper la ligne dans cmdtoks
        int n;
        if ((n = strcut(TMPCMDLINE, ' ', cmdtoks, MAX_CMD_SIZE)) <= 0) {
            //printf("strcut Fail");
            continue;
        }
        /*
        for (int i = 0; i < MAX_CMD_SIZE; i++) {
            if (cmdtoks[i] != NULL) {
                printf("tokens[%d]:\"%s\"\n", i, cmdtoks[i]);
           }
       }
        */

        // Traduire la ligne en structures cmd_t dans cmds
        if (parse_cmd(cmdtoks, cmds, n) != 0) {
            //printf("Parse Fail");
            continue;
        }

        // For debugging
        /*
        for (int i = 0; i < 10; ++i) {
        printf("Cmd %d: path = %s\n", i, cmds[i].path);
            for (int j = 0; cmds[i].argv[j] != NULL; ++j) {
                    printf("    argv[%d] = %s\n", j, cmds[i].argv[j]);
            }
              printf("Next = %p NextSuc = %p NextFail = %p\n", (void*)cmds[i].next, (void*)cmds[i].next_success, (void*)cmds[i].next_failure);
        }
        */

        // Les commandes sont chaînées en fonction des séparateurs
        //   - next -> exécution inconditionnelle
        //   - next_success -> exécution si la commande précédente réussit
        //   - next_failure -> exécution si la commande précédente échoue

        // Exécuter les commandes dans l'ordre en fonction des opérateurs
        // de flux
        current = cmds; // Commencer par la première commande
        while (current != NULL) {
            // Lancer la commande
            int result2;
            if (current->path == NULL) {
                break;
            } else if (is_builtin(current->path)) {
                result2 = builtin(current);
            } else {
                result2 = exec_cmd(current);
            }
	
            // Passer à la structure de commande suivante
            if (current->next_success != NULL || current->next_failure != NULL) {
                
                if (current->status == 0 && result2 == 0) {
                    
                    current = current->next_success;
                } else {
                    
                    current = current->next_failure;
                }
            } else if (current->next != NULL) {
                    
                current = current->next;
            } else {    
                    
                break; // Plus de commandes, sortir de la boucle
            }
        }
    }
    free(TMPCMDLINE);
    // Libération de la mémoire allouée pour l'historique
    freehistory();
    return 0;
}

