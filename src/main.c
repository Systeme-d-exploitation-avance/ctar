#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/archive.h"

int main(int argc, char *argv[])
{
    int opt;
    char *archive_path = NULL;
    char *output_archive = NULL;
    char *output_directory = ".";

    while ((opt = getopt(argc, argv, "lxc:z:")) != -1)
    {
        switch (opt)
        {
        case 'l':
        case 'x':
            // FM01 - Listage des fichiers de l'archive
            // FM02 - Extraction de l'archive
            if (optind < argc)
            {
                archive_path = argv[optind];
                optind++;
            }
            else
            {
                fprintf(stderr, "Error: No archive path specified.\n");
                exit(EXIT_FAILURE);
            }

            if (opt == 'x' && optind < argc)
            {
                output_directory = argv[optind];
            }

            if (opt == 'l')
            {
                list_files(archive_path);
            }
            else if (opt == 'x')
            {
                extract_archive_tar_gz(archive_path, output_directory);
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
            create_archive_tar(output_archive, input_files, num_files);
            break;

        case 'z':
            if (optarg != NULL)
            {
                archive_path = optarg;
                printf("archive_path: %s\n", archive_path);

                if (optind < argc)
                {
                    output_archive = argv[optind];
                    printf("output_archive: %s\n", output_archive);
                    optind++; // Increment optind manually
                }
                else
                {
                    fprintf(stderr, "Error: No result archive path specified.\n");
                    exit(EXIT_FAILURE);
                }

                compress_tar_to_gz(archive_path, output_archive);
            }
            else
            {
                fprintf(stderr, "Error: No source archive path specified.\n");
                exit(EXIT_FAILURE);
            }
            break;

        default:
            fprintf(stderr, "Usage: %s [-lxc archive_path]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
