// archive.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "archive.h"
#include "typedef.h"
#include <errno.h>
#include <zlib.h>
#include <sys/stat.h>

char buffer[BLOCK_SIZE];

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

// Fonction pour créer un dossier
int create_directory(const char *path)
{
    if (mkdir(path, 0777) != 0 && errno != EEXIST)
    {
        perror("Erreur lors de la création du répertoire extrait");
        return 0; // Échec
    }
    return 1; // Succès
}

void extract_archive(const char *archive_path, const char *output_dir)
{
    gzFile archive = gzopen(archive_path, "rb");
    if (archive == NULL)
    {
        perror("Erreur lors de l'ouverture de l'archive");
        exit(EXIT_FAILURE);
    }

    char buffer[BLOCK_SIZE];

    // Vérifier si le répertoire de sortie existe
    if (!create_directory(output_dir))
    {
        fprintf(stderr, "Erreur lors de la création du répertoire de sortie");
        exit(EXIT_FAILURE);
    }

    // Boucle de lecture des entêtes des fichiers dans l'archive
    while (gzread(archive, buffer, BLOCK_SIZE) > 0)
    {
        struct header_tar *file_header = (struct header_tar *)buffer;

        // Vérifier si l'entête est un bloc rempli de zéros, indiquant la fin de l'archive
        if (memcmp(file_header->name, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 20) == 0)
        {
            break; // Fin de l'archive
        }

        // Extrait le fichier
        char output_path[PATH_MAX];
        snprintf(output_path, sizeof(output_path), "%s/%s", output_dir, file_header->name);

        if (file_header->name[strlen(file_header->name) - 1] == '/')
        {
            // Le fichier est un répertoire, crée-le s'il n'existe pas
            if (!create_directory(output_path))
            {
                fprintf(stderr, "Erreur lors de la création du répertoire extrait");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            // Le fichier est un fichier normal, crée-le
            FILE *output_file = fopen(output_path, "wb");
            if (output_file == NULL)
            {
                perror("Erreur lors de la création du fichier extrait");
                exit(EXIT_FAILURE);
            }

            // Boucle de lecture et écriture du contenu du fichier
            int remaining_size = octal_to_int(file_header->size);
            while (remaining_size > 0)
            {
                int read_size = gzread(archive, buffer, sizeof(buffer));
                if (read_size <= 0)
                {
                    perror("Erreur lors de la lecture du fichier compressé");
                    exit(EXIT_FAILURE);
                }

                int write_size = fwrite(buffer, 1, read_size, output_file);
                if (write_size != read_size)
                {
                    perror("Erreur lors de l'écriture du fichier extrait");
                    exit(EXIT_FAILURE);
                }

                remaining_size -= read_size;
            }

            fclose(output_file);
        }
    }

    gzclose(archive);
}

void create_archive(const char *output_archive, const char *input_files[], int num_files)
{
}

size_t filesize(const char *filename)
{
    struct stat st;
    if (stat(filename, &st) != 0)
    {
        perror("Erreur lors de la récupération de la taille du fichier");
        exit(EXIT_FAILURE);
    }
    return (size_t)st.st_size;
}

int octal_to_int(const char *octal_str)
{
    return strtol(octal_str, NULL, 8);
}