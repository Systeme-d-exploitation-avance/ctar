// archive.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "archive.h"
#include "typedef.h"
#include <errno.h>
#include <zlib.h>

void list_files(const char *archive_path)
{
    gzFile archive = gzopen(archive_path, "rb");
    if (archive == NULL)
    {
        perror("Erreur lors de l'ouverture de l'archive");
        fprintf(stderr, "errno = %d\n", errno);
        fprintf(stderr, "Chemin d'accès au fichier : %s\n", archive_path);
        exit(EXIT_FAILURE);
    }

    char buffer[BLOCK_SIZE];

    // Boucle de lecture des entêtes des fichiers dans l'archive
    while (gzread(archive, buffer, BLOCK_SIZE) > 0)
    {
        struct header_tar *file_header = (struct header_tar *)buffer;

        // Vérifier si l'entête est un bloc rempli de zéros, indiquant la fin de l'archive
        if (memcmp(file_header->name, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 20) == 0)
        {
            break; // Fin de l'archive
        }

        // Afficher le nom du fichier de l'entête
        printf("%s\n", file_header->name);

        // Se déplacer à la position suivante de l'entête
        gzseek(archive, BLOCK_SIZE - sizeof(struct header_tar), SEEK_CUR);
    }

    gzclose(archive);
}

void extract_archive(const char *archive_path)
{
    (void)archive_path;
    // Implémentation de la fonction pour extraire l'archive
}

void create_archive(const char *output_archive, const char *input_files[], int num_files)
{
    (void)output_archive;
    (void)input_files;
    (void)num_files;
    // Implémentation de la fonction pour créer une archive
}
