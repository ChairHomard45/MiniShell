/*
  Projet minishell - Licence 3 Info - PSI 2023
 
  Nom : Delepaut Courtin-Coin
  Prénom : Clement Théo
  Num. étudiant : 22108706 22100975
  Groupe de projet : 15
  Date : 24 / 11 / 2023

  Fichier builtin.c :Gestion des commandes internes du minishell (implémentation).
  Dépendances : cmd.h buitlin.h
  
 */

#include "cmd.h"
#include "builtin.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


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

//Libère la mémoire allouée
void freehistory(){
    for (int i = 0; i < history_index; i++) {
    	free(history[i]);
    	}
}

// Tableau de chaînes statiques pour les descriptions des commandes
static const char* command_descriptions[] = {
    "cd : Change le répertoire de travail courant.",
    "exit : Quitte le minishell.",
    "export : Ajoute/modifie une variable d'environnement.",
    "unset : Enlève la valeur associée à la variable d'environnement.",
    "help : Affiche l'aide ou la documentation du programme.",
    // Redirection et Opérateur.
    "! : Permet l'appel d'une antérieur en utilisant son numéro dans history",
    "> : Redirection de la sortie standard vers un fichier (écrasement).",
    "< : Redirection de l'entrée standard depuis un fichier.",
    ">> : Redirection de la sortie standard vers un fichier (ajout).",
    "2> : Redirection de la sortie d'erreur vers un fichier (écrasement).",
    "2>> : Redirection de la sortie d'erreur vers un fichier (ajout).",
    "&> : Redirection de la sortie standard et d'erreur vers un fichier (écrasement).",
    "| : Pipe, permet la communication entre deux processus.",
    "&& : Opérateur logique AND.",
    "|| : Opérateur logique OR.",
    "2>&1 : Redirection de la sortie d'erreur vers la sortie standard.",
    "; : Sépare plusieurs commandes sur la même ligne.",
    NULL // Marqueur de fin du tableau
};

// Affiche la description de la commande si elle est dans le tableau
void print_command_description() {
    printf("\nCommand isbuiltin Help:\n");
    int i = 0;
    while(command_descriptions[i] != NULL && i != 5){
        printf("%d: %s\n", i + 1, command_descriptions[i]);
        i++;
    }
    printf("\nOpérateurs Implémentés:\n");
    while(command_descriptions[i] != NULL){
        printf("%s\n", command_descriptions[i]);
        i++;
    }
    printf("\n");
}

//
int execute_previous_command(char* input) { 
   if (strncmp(input, "!", 1) == 0) { 
	int command_number = atoi(input + 1); // Récupérer le numéro de commande après '!'
		// Vérifier si l'utilisateur a saisi un nombre valide après '!' 
		if (command_number > 0 && command_number <= history_index) { 
		// Exécuter la commande correspondante dans l'historique 
			printf("Executing previous command: %s\n", history[command_number - 1]);
			strcpy(input,history[command_number - 1]);
			// Ici, vous devriez exécuter la commande dans le shell return 1; // Succès 
		} else { printf("Command number out of range or invalid\n"); return -1; 
		// Échec 
		} 
    } return 0; // Pas de commande précédente à exécuter 
}



// Les méthodes originales

// Vérifie si une commande est interne ou non
int is_builtin(const char* cmd) {
    if (strcmp(cmd, "cd") == 0 || strcmp(cmd, "exit") == 0 || strcmp(cmd, "export") == 0 || strcmp(cmd, "unset") == 0) {
        return 1; // La commande est une commande interne
    }
    return 0; // La commande n'est pas une commande interne
}

// Exécute la commande interne correspondante
int builtin(cmd_t* cmd) {
    if (strcmp(cmd->path, "cd") == 0) {
        return cd(cmd->argv[1], cmd->sterr);
    } else if (strcmp(cmd->path, "exit") == 0) {
        return exit_shell(0, cmd->stout);
    } else if (strcmp(cmd->path, "export") == 0) {
        return export(cmd->argv[1], cmd->argv[2], cmd->sterr);
    } else if (strcmp(cmd->path, "unset") == 0) {
        return unset(cmd->argv[1]);
    }
    return -1; // Commande non reconnue
}

// Change le répertoire de travail
int cd(const char* path, int fderr) {
    if(path == NULL){
        int result = write(fderr, "Error changement de répertoire: Path NULL", strlen("Error changement de répertoire: Path NULL"));
        (void)result;
        return -1;
    }
    if (chdir(path) == -1) {
        dprintf(fderr, "%s", path);
        int result = write(fderr, "Error changement de répertoire", strlen("Error changement de répertoire"));
        (void)result;
        return -1;
    }
    return 0;
}

// Modifie ou ajoute des variables d'environnement
int export(const char* var, const char* value, int fderr) {
    if (!var || !value) {
        int result = write(fderr, "Error pas assez d'argument", strlen("Error pas assez d'argument"));
        (void)result;
        return 1;
    }
    if (setenv(var, value, 1) == -1) {
        int result = write(fderr, "Error: Could not set environment variable", strlen("Error: Could not set environment variable"));
        (void)result;
        return -1;
    }
    return 0;
}

// Supprime une variable d'environnement
int unset(const char* var) {
    unsetenv(var);
    return 0;
}

// Quitte le shell
int exit_shell(int ret, int fderr) {
    int result = write(fderr, "Goodbye!\n", strlen("Goodbye!\n"));
    (void)result;
    exit(ret);
}

