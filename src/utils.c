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

void check_file_open_error(void* file, const char *filePath)
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

// Function to calculate and update the checksum
void calculate_checksum(struct header_tar *header)
{
    unsigned int checksum = 0;
    for (size_t i = 0; i < sizeof(struct header_tar); ++i)
    {
        checksum += ((unsigned char *)header)[i];
    }
    snprintf(header->checksum, sizeof(header->checksum), "%06o", checksum);
}

// Function to add padding to the archive
void add_padding(gzFile archive, int size)
{
    char buffer[BLOCK_SIZE];
    memset(buffer, 0, size);
    gzwrite(archive, buffer, size);
}

// Function to create and write the header to the archive
void write_header(gzFile archive, const char *filename, int is_directory)
{
    struct header_tar file_header;
    memset(&file_header, 0, sizeof(struct header_tar));

    strncpy(file_header.name, filename, sizeof(file_header.name));

    if (is_directory) {
        file_header.typeflag = '5'; // '5' for directories
    } else {
        sprintf(file_header.mode, "%07o", 0644);
        sprintf(file_header.uid, "%07o", 0);
        sprintf(file_header.gid, "%07o", 0);
        sprintf(file_header.size, "%011o", get_file_size(filename));
        file_header.typeflag = '0'; // '0' for regular files
    }

    sprintf(file_header.mtime, "%011o", (unsigned int)time(NULL));
    memset(file_header.checksum, ' ', sizeof(file_header.checksum));
    strcpy(file_header.magic, "ustar ");
    strcpy(file_header.version, "00");
    file_header.linkname[0] = '\0';
    file_header.uname[0] = '\0';
    file_header.gname[0] = '\0';
    file_header.devmajor[0] = '\0';
    file_header.devminor[0] = '\0';
    file_header.prefix[0] = '\0';
    file_header.padding[0] = '\0';

    calculate_checksum(&file_header);

    gzwrite(archive, &file_header, sizeof(file_header));
}

void add_padding_tar(FILE *archive, int size) {
    char buffer[BLOCK_SIZE];
    memset(buffer, 0, size);
    fwrite(buffer, 1, size, archive);
}

void write_header_tar(FILE *archive, const char *filename, int is_directory) {
    struct header_tar file_header;
    memset(&file_header, 0, sizeof(struct header_tar));

    strncpy(file_header.name, filename, sizeof(file_header.name));

    if (is_directory) {
        file_header.typeflag = '5'; // '5' for directories
    } else {
        sprintf(file_header.mode, "%07o", 0644);
        sprintf(file_header.uid, "%07o", 0);
        sprintf(file_header.gid, "%07o", 0);
        sprintf(file_header.size, "%011o", get_file_size(filename));
        file_header.typeflag = '0'; // '0' for regular files
    }

    sprintf(file_header.mtime, "%011o", (unsigned int)time(NULL));
    memset(file_header.checksum, ' ', sizeof(file_header.checksum));
    strcpy(file_header.magic, "ustar ");
    strcpy(file_header.version, "00");
    file_header.linkname[0] = '\0';
    file_header.uname[0] = '\0';
    file_header.gname[0] = '\0';
    file_header.devmajor[0] = '\0';
    file_header.devminor[0] = '\0';
    file_header.prefix[0] = '\0';
    file_header.padding[0] = '\0';

    calculate_checksum(&file_header);

    fwrite(&file_header, 1, sizeof(file_header), archive);
}

void create_parent_directories(const char *path)
{
    char *parent = strdup(path);
    char *parentDir = dirname(parent);

    // Recursively create parent directories
    char *token = strtok(parentDir, "/");
    char currentDir[PATH_MAX];
    currentDir[0] = '\0';

    while (token != NULL)
    {
        strcat(currentDir, token);
        if (mkdir(currentDir, 0777) != 0 && errno != EEXIST)
        {
            handle_error("Erreur lors de la création des répertoires parents");
        }

        strcat(currentDir, "/");
        token = strtok(NULL, "/");
    }

    free(parent);
}