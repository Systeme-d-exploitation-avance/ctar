// main.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "archive.h"

int main(int argc, char *argv[]) {
    // Récupération des paramètres de la ligne de commande
    int opt;
    char *archive_path = NULL;

    while ((opt = getopt(argc, argv, "lxc:")) != -1) {
        switch (opt) {
            case 'l':
                // FM01 - Listage des fichiers de l'archive
                if (optind < argc) {
                    archive_path = argv[optind]; // Utilise le prochain argument comme chemin d'accès
                } else {
                    fprintf(stderr, "Erreur: Aucun chemin d'accès spécifié.\n");
                    exit(EXIT_FAILURE);
                }
                list_files(archive_path);
                break;
            case 'x':
                // FM02 - Extraction de l'archive
                // À implémenter dans la prochaine étape
                break;
            case 'c':
                // FM03 - Création d'une archive
                // À implémenter dans les étapes ultérieures
                break;
            default:
                fprintf(stderr, "Usage: %s [-lxc archive_path]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    return 0;
}
