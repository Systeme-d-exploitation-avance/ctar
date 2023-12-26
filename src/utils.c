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

void checkFileOpenError(gzFile file, const char *filePath)
{
    if (file == NULL)
    {
        perror("Erreur lors de l'ouverture de l'archive");
        fprintf(stderr, "errno = %d\n", errno);
        fprintf(stderr, "Chemin d'accès au fichier : %s\n", filePath);
        exit(EXIT_FAILURE);
    }
}

int isEndOfArchive(struct header_tar *fileHeader)
{
    return memcmp(fileHeader->name, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 20) == 0;
}