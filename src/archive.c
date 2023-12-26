// archive.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/archive.h"
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

int get_file_size(const char *file_path)
{
    struct stat st;
    if (stat(file_path, &st) == 0)
    {
        return st.st_size;
    }
    return -1; // Erreur
}

void create_archive(const char *output_archive, const char *input_files[], int num_files)
{
    gzFile archive = gzopen(output_archive, "wb");
    if (archive == NULL)
    {
        perror("Erreur lors de la création de l'archive");
        exit(EXIT_FAILURE);
    }

    char buffer[BLOCK_SIZE];

    // Boucle pour chaque fichier à ajouter à l'archive
    for (int i = 0; i < num_files; ++i)
    {
        FILE *input_file = fopen(input_files[i], "rb");
        if (input_file == NULL)
        {
            perror("Erreur lors de l'ouverture du fichier à ajouter à l'archive");
            exit(EXIT_FAILURE);
        }

        // Créer l'entête du fichier
        struct header_tar file_header;
        memset(&file_header, 0, sizeof(struct header_tar)); // Initialize the header with zeros

        // Set the fields in the header
        strncpy(file_header.name, input_files[i], sizeof(file_header.name));
        sprintf(file_header.mode, "%07o", 0644); // Mode par défaut pour les fichiers
        sprintf(file_header.uid, "%07o", 0);     // UID par défaut
        sprintf(file_header.gid, "%07o", 0);     // GID par défaut
        sprintf(file_header.size, "%011o", get_file_size(input_files[i]));
        sprintf(file_header.mtime, "%011o", (unsigned int)time(NULL));
        memset(file_header.checksum, ' ', sizeof(file_header.checksum));
        file_header.typeflag = '0'; // Typeflag '0' pour les fichiers normaux
        strcpy(file_header.magic, "ustar ");
        strcpy(file_header.version, "00");
        file_header.linkname[0] = '\0';
        file_header.uname[0] = '\0';
        file_header.gname[0] = '\0';
        file_header.devmajor[0] = '\0';
        file_header.devminor[0] = '\0';
        file_header.prefix[0] = '\0';
        file_header.padding[0] = '\0';

        // Calculer et mettre à jour le checksum de l'entête
        unsigned int checksum = 0;
        for (size_t j = 0; j < sizeof(file_header); ++j)
        {
            checksum += ((unsigned char *)&file_header)[j];
        }
        snprintf(file_header.checksum, sizeof(file_header.checksum), "%06o", checksum);

        // Écrire l'entête dans l'archive
        gzwrite(archive, &file_header, sizeof(file_header));

        // Boucle de lecture et écriture du contenu du fichier
        int read_size;
        while ((read_size = fread(buffer, 1, sizeof(buffer), input_file)) > 0)
        {
            gzwrite(archive, buffer, read_size);
        }

        // Fermer le fichier d'entrée
        fclose(input_file);

        // Ajouter des blocs de remplissage si nécessaire
        int padding_size = BLOCK_SIZE - (get_file_size(input_files[i]) % BLOCK_SIZE);
        if (padding_size < BLOCK_SIZE)
        {
            memset(buffer, 0, padding_size);
            gzwrite(archive, buffer, padding_size);
        }
    }

    // Ajouter les blocs de 512 octets remplis de blanc binaire pour marquer la fin de l'archive
    memset(buffer, 0, sizeof(buffer));
    gzwrite(archive, buffer, BLOCK_SIZE); // Ajout d'un bloc de remplissage supplémentaire
    gzwrite(archive, buffer, BLOCK_SIZE); // Ajout du deuxième bloc de remplissage pour marquer la fin

    gzclose(archive);
}

int octal_to_int(const char *octal_str)
{
    return strtol(octal_str, NULL, 8);
}