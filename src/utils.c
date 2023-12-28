/// \file
/// \brief Utility functions

#include "../include/utils.h"

// Function to create a directory if it doesn't exist
int create_directory(const char *path)
{
    if (mkdir(path, 0777) != 0 && errno != EEXIST)
    {
        perror("Erreur lors de la création du répertoire extrait");
        return 0;
    }
    return 1;
}

// Function to get the size of a file
int get_file_size(const char *file_path)
{
    struct stat st;
    if (stat(file_path, &st) == 0)
    {
        return st.st_size;
    }
    return -1;
}

// Function to convert an octal string to an integer
int octal_to_int(const char *octal_str)
{
    return strtol(octal_str, NULL, 8);
}

// Function to check if a file is openned correctly and handle the error if not
void check_file_open_error(void *file, const char *filePath)
{
    if (file == NULL)
    {
        handle_error("Erreur lors de l'ouverture du fichier");
        printf("Fichier : %s\n", filePath);
    }
}

// Function to handle errors using errno codes and exit the program
void handle_error(const char *message)
{
    perror(message);

    switch (errno)
    {
    case EEXIST:
        fprintf(stderr, "Error: File already exists.\n");
        break;
    case EFAULT:
        fprintf(stderr, "Error: Bad address.\n");
        break;
    case EISDIR:
        fprintf(stderr, "Error: Is a directory.\n");
        break;
    case ENOTDIR:
        fprintf(stderr, "Error: Not a directory.\n");
        break;
    case ELOOP:
        fprintf(stderr, "Error: Too many levels of symbolic links.\n");
        break;

    default:
        fprintf(stderr, "Unknown error.\n");
    }

    exit(EXIT_FAILURE);
}

// Function to open a gzip archive using zlib
gzFile open_archive(const char *archive_path)
{
    gzFile archive = gzopen(archive_path, "rb");
    if (archive == NULL)
    {
        handle_error("Erreur lors de l'ouverture de l'archive");
    }
    return archive;
}

// Function to check if its the end of the archive
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

// Function to add padding to the gzip archive
void add_padding_gz(gzFile archive, int size)
{
    char buffer[BLOCK_SIZE];
    memset(buffer, 0, size);
    gzwrite(archive, buffer, size);
}

// Function to create and write the header to the gzip archive
void write_header_gz(gzFile archive, const char *filename, int is_directory)
{
    struct header_tar file_header;
    memset(&file_header, 0, sizeof(struct header_tar));

    strncpy(file_header.name, filename, sizeof(file_header.name));

    if (is_directory)
    {
        file_header.typeflag = '5'; // '5' for directories
    }
    else
    {
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

// Function to add padding to the archive
void add_padding(FILE *archive, int size)
{
    char buffer[BLOCK_SIZE];
    memset(buffer, 0, size);
    fwrite(buffer, 1, size, archive);
}

// Function to create and write the header to the archive
void write_header(FILE *archive, const char *filename, int is_directory)
{
    struct header_tar file_header;
    memset(&file_header, 0, sizeof(struct header_tar));

    strncpy(file_header.name, filename, sizeof(file_header.name));

    if (is_directory)
    {
        file_header.typeflag = '5'; // '5' for directories
    }
    else
    {
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

// Function to skip to the next header in the archive
void skip_to_next_header(gzFile archive, size_t fileSize)
{
    // Calculate the number of blocks for the file content
    size_t numBlocks = (fileSize + BLOCK_SIZE - 1) / BLOCK_SIZE;
    gzseek(archive, numBlocks * BLOCK_SIZE, SEEK_CUR);
}