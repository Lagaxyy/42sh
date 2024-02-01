#include "exec_ast.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fonction pour exécuter la commande "cd"
int execute_cd(struct Ast *ast) {
    if (ast == NULL)
        return 1; // Retourner 1 en cas d'erreur

    // Récupérer le chemin du répertoire à changer
    char *path = ast->data.sc.cmd->tokenlist[1]->text; // Assurez-vous que l'index est correct

    // Obtenir le répertoire de travail actuel (avant le changement)
    char *old_working_dir = getcwd(NULL, 0);

    // Exécuter la commande "cd -" si le chemin est égal à "-"
    if (strcmp(path, "-") == 0) {
        char *previous_dir = getenv("OLDPWD");
        if (previous_dir != NULL) {
            if (chdir(previous_dir) != 0) {
                free(old_working_dir);
                return 1; // Retourner 1 en cas d'erreur
            } else {
                // Mettre à jour OLDPWD et PWD après le changement de répertoire
                setenv("OLDPWD", old_working_dir, 1);
                setenv("PWD", previous_dir, 1);
                free(old_working_dir);
                return 0; // Retourner 0 en cas de succès
            }
        } else {
            free(old_working_dir);
            return 1; // Retourner 1 en cas d'erreur
        }
    } else {
        // Exécuter la commande "cd" avec le chemin spécifié
        if (chdir(path) != 0) {
            free(old_working_dir);
            return 1; // Retourner 1 en cas d'erreur
        } else {
            // Mettre à jour OLDPWD et PWD après le changement de répertoire
            setenv("OLDPWD", old_working_dir, 1);
            setenv("PWD", path, 1);
            free(old_working_dir);
            return 0; // Retourner 0 en cas de succès
        }
    }
}

// int main() {
//     // Exemple d'utilisation
//     struct Token *token = NULL; // Définir le token si nécessaire
//     struct Command *command = malloc(sizeof(struct Command));
//     command->tokenlist = malloc(sizeof(struct Token *) * 2);
//     command->tokenlist[0] = token; // Ajouter le premier token
//     command->tokenlist[1] = NULL;  // Assurez-vous que le dernier élément du tableau est NULL
//     command->nbtoken = 1;

//     struct Ast_simple_cmd ast_cmd;
//     ast_cmd.cmd = command;

//     struct Ast *ast = malloc(sizeof(struct Ast));
//     ast->type = VOTRE_TYPE_AST_CD; // Remplacez par le type d'AST correspondant à la commande "cd"
//     ast->data.sc = ast_cmd;
//     ast->next = NULL;

//     // Appeler la fonction pour exécuter la commande "cd"
//     execute_cd(ast);

//     // Libérer la mémoire
//     free(command->tokenlist);
//     free(command);
//     free(ast);

//     return 0;
// }
