// main.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/archive.h"

int main(int argc, char *argv[])
{
    // Récupération des paramètres de la ligne de commande
    int opt;
    char *archive_path = NULL;
    char *output_archive = NULL;
    char *output_directory = "."; // Valeur par défaut pour l'extraction

    while ((opt = getopt(argc, argv, "lxc:")) != -1)
    {
        switch (opt)
        {
        case 'l':
        case 'x':
            // FM01 - Listage des fichiers de l'archive
            // FM02 - Extraction de l'archive
            if (optind < argc)
            {
                archive_path = argv[optind]; // Utilise le prochain argument comme chemin d'accès
                optind++; // Incrémente l'indice pour l'option '-x' qui a un argument supplémentaire
            }
            else
            {
                fprintf(stderr, "Erreur: Aucun chemin d'accès spécifié.\n");
                exit(EXIT_FAILURE);
            }

            if (opt == 'x' && optind < argc)
            {
                // Si l'option est 'x' et un deuxième argument est spécifié, utilise-le comme répertoire de sortie
                output_directory = argv[optind];
            }

            if (opt == 'l')
            {
                list_files(archive_path);
            }
            else if (opt == 'x')
            {
                extract_archive_tar(archive_path, output_directory);
            }
            break;

        case 'c':
            // FM03 - Création d'une archive
            if (optarg != NULL)
            {
                // optarg contient le nom de l'archive à créer
                output_archive = optarg;
            }
            else
            {
                fprintf(stderr, "Erreur: L'option -c nécessite un nom d'archive.\n");
                exit(EXIT_FAILURE);
            }

            // Les arguments restants sont les fichiers à inclure
            const char **input_files = (const char **)&argv[optind];
            int num_files = argc - optind;

            // Affiche le nom de l'archive et la liste des fichiers pour le débogage
            printf("Nom de l'archive: %s\n", output_archive);
            printf("Liste des fichiers:\n");
            for (int i = 0; i < num_files; ++i)
            {
                printf("   %s\n", input_files[i]);
            }

            // Appelle la fonction pour créer l'archive
            create_archive(output_archive, input_files, num_files);
            break;

        default:
            fprintf(stderr, "Usage: %s [-lxc archive_path]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
