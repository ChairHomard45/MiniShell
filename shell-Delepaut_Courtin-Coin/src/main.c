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

#define MAX_HISTORY_SIZE 100

char* history[MAX_HISTORY_SIZE];
int history_index = 0;

// Ajouter une commande à l'historique
void add_to_history(const char* cmd) {
    if (history_index < MAX_HISTORY_SIZE) {
        history[history_index++] = strdup(cmd);
    } else {
        free(history[0]);
        for (int i = 0; i < MAX_HISTORY_SIZE - 1; i++) {
            history[i] = history[i + 1];
        }
        history[MAX_HISTORY_SIZE - 1] = strdup(cmd);
    }
}

// Afficher l'historique des commandes
void print_history() {
    printf("\nCommand History:\n");
    for (int i = 0; i < history_index; i++) {
        printf("%d: %s\n", i + 1, history[i]);
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
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

    int result2;

    while (1) {
        printf("\nDebut\n");

        // Effacer les contenus de cmdline, cmdtoks et cmds
        cmdline[0] = '\0';
        for (int i = 0; i < MAX_CMD_SIZE; i++) {
            if (cmdtoks[i] != NULL) {
                cmdtoks[i] = '\0';
            }
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
        }

        // Ajouter la commande à l'historique
        add_to_history(cmdline);

        // Deallocation de la mémoire allouée dans la boucle
        if (TMPCMDLINE != NULL) {
            free(TMPCMDLINE);
            TMPCMDLINE = NULL; // Réinitialiser pour éviter un pointeur errant
        }
        TMPCMDLINE = strdup(cmdline);

        // Affiche la chaîne originale
        printf("Original:\"%s\"\n", TMPCMDLINE);

        // Traiter la ligne de commande
        //   - supprimer les espaces en début et en fin de ligne
        if (trim(TMPCMDLINE) <= 0) {
            printf("Trim Fail");
            continue;
        }
        printf("Trim:\"%s\"\n", TMPCMDLINE);

        //   - ajouter d'éventuels espaces autour de ; ! || && & ...
        if (separate_s(TMPCMDLINE, "|&<>!;2", MAX_CMD_SIZE) != 0){
            printf("Separate Fail");
            continue;
        }
        printf("Separate:\"%s\"\n", TMPCMDLINE);

        //   - supprimer les doublons d'espaces
        if (clean(TMPCMDLINE) <= 0) {
            printf("Clean Fail");
            continue;
        }
        printf("clean:\"%s\"\n", TMPCMDLINE);

        //   - traiter les variables d'environnement
        if (substenv(TMPCMDLINE, MAX_CMD_SIZE) < 0) {
            printf("Substenv Fail");
            continue;
        }
        printf("Substenv:\"%s\"\n", TMPCMDLINE);

        // Découper la ligne dans cmdtoks
        int n;
        if ((n = strcut(TMPCMDLINE, ' ', cmdtoks, MAX_CMD_SIZE)) <= 0) {
            printf("strcut Fail");
            continue;
        }

        for (int i = 0; i < MAX_CMD_SIZE; i++) {
            if (cmdtoks[i] != NULL) {
                printf("tokens[%d]:\"%s\"\n", i, cmdtoks[i]);
            }
        }

        // Traduire la ligne en structures cmd_t dans cmds
        if (parse_cmd(cmdtoks, cmds, n) != 0) {
            printf("Parse Fail");
            continue;
        }

        // Les commandes sont chaînées en fonction des séparateurs
        //   - next -> exécution inconditionnelle
        //   - next_success -> exécution si la commande précédente réussit
        //   - next_failure -> exécution si la commande précédente échoue

        // Exécuter les commandes dans l'ordre en fonction des opérateurs
        // de flux
        current = cmds; // Commencer par la première commande
        while (current != NULL) {
            // Lancer la commande
            if (current->path == NULL) {
                break;
            } else if (is_builtin(current->path)) {
                result2 = builtin(current);
            } else {
                result2 = exec_cmd(current);
            }

            // Vérifier le résultat de l'exécution de la commande
            if (result2 != 0) {
                break; // Sortir de la boucle si l'exécution de la commande échoue
            }

            printf("Path used =%s\n", current->path);

            // Passer à la structure de commande suivante
            if (current->next_success != NULL && current->path != NULL && current->status == 0) {
                current = current->next_success;
                printf("here1: current succes %s %s\n", current->path,current->argv[0]);
            } else if (current->next_failure != NULL && current->path != NULL  && current->status != 0) {
                current = current->next_failure;
                printf("here2: %s\n", current->path);
            } else if (current->next != NULL && current->path != NULL) {
                current = current->next;
                printf("here3: current is not NULL %s %s\n", current->path,current->argv[1]);
            } else {
                printf("here4: current is NULL\n");
                break; // Plus de commandes, sortir de la boucle
            }
        }
    }
    free(TMPCMDLINE);
    // Libération de la mémoire allouée pour l'historique
    for (int i = 0; i < history_index; i++) {
        free(history[i]);
    }
    return 0;
}

