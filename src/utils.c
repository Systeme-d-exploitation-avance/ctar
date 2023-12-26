// utils.c

#include "../include/utils.h"

int create_directory(const char *path)
{
    if (mkdir(path, 0777) != 0 && errno != EEXIST)
    {
        perror("Erreur lors de la création du répertoire extrait");
        return 0; // Échec
    }
    return 1; // Succès
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

int octal_to_int(const char *octal_str)
{
    return strtol(octal_str, NULL, 8);
}

void check_file_open_error(gzFile file, const char *filePath)
{
    if (file == NULL)
    {
        perror("Erreur lors de l'ouverture de l'archive");
        fprintf(stderr, "errno = %d\n", errno);
        fprintf(stderr, "Chemin d'accès au fichier : %s\n", filePath);
        exit(EXIT_FAILURE);
    }
}

// Function to handle errors and exit
void handle_error(const char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

// Function to open a gzip archive
gzFile open_archive(const char *archive_path)
{
    gzFile archive = gzopen(archive_path, "rb");
    if (archive == NULL)
    {
        handle_error("Erreur lors de l'ouverture de l'archive");
    }
    return archive;
}

bool is_end_of_archive(const char *name)
{
    // Check if the name field is filled with zeros
    return memcmp(name, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 20) == 0;
}
