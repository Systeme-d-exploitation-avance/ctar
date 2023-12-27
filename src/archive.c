// archive.c

#include "../include/archive.h"

char buffer[BLOCK_SIZE];

// FM01 - Listage des fichiers de l'archive (tar et gz)
// Function to list the files in the archive
void list_files(const char *archivePath)
{
    // Open the archive
    gzFile archive = gzopen(archivePath, "rb");
    check_file_open_error(archive, archivePath);

    char buffer[BLOCK_SIZE];

    // Read the archive header by header
    while (gzread(archive, buffer, BLOCK_SIZE) > 0)
    {
        // Get the header from the buffer
        struct header_tar *fileHeader = (struct header_tar *)buffer;

        if (is_end_of_archive(fileHeader->name))
        {
            break; // End of the archive
        }

        // Display the name of the file from the header
        printf("%s\n", fileHeader->name);

        // Move to the next header position
        gzseek(archive, BLOCK_SIZE - sizeof(struct header_tar), SEEK_CUR);
    }

    gzclose(archive);
}

// FM02 - Extraction de l'archive (tar)
// Function to extract a file from the archive (tar)
void extract_file_tar(FILE *archive, const char *outputPath, int fileSize)
{
    // Open the output file
    FILE *outputFile = fopen(outputPath, "wb");
    if (outputFile == NULL)
    {
        handle_error("Error creating the extracted file");
    }

    // Read the file content from the archive and write it to the output file
    while (fileSize > 0)
    {
        // Read the file content from the archive
        int readSize = fread(buffer, 1, sizeof(buffer), archive);
        if (readSize <= 0)
        {
            handle_error("Error reading the compressed file");
        }

        int writeSize = fwrite(buffer, 1, readSize, outputFile);
        if (writeSize != readSize)
        {
            handle_error("Error writing the extracted file");
        }

        fileSize -= readSize;
    }

    fclose(outputFile);
}

// FM02 - Extraction de l'archive (tar)
// Function to extract the archive (tar)
void extract_archive_tar(const char *archivePath, const char *outputDir)
{
    // Open the archive
    FILE *archive = fopen(archivePath, "rb");
    check_file_open_error(archive, archivePath);

    // Create the output directory
    if (!create_directory(outputDir))
    {
        handle_error("Error creating the output directory");
    }

    // Read the archive header by header
    while (fread(buffer, 1, BLOCK_SIZE, archive) > 0)
    {
        // Get the header from the buffer
        struct header_tar *fileHeader = (struct header_tar *)buffer;

        // Check if it's the end of the archive
        if (is_end_of_archive(fileHeader->name))
        {
            break; // End of the archive
        }

        // Get the file size from the header
        char outputPath[PATH_MAX];
        // Create the output path
        snprintf(outputPath, sizeof(outputPath), "%s/%s", outputDir, fileHeader->name);

        // Extract the file or create the directory
        if (fileHeader->typeflag == '5')
        {
            if (!create_directory(outputPath))
            {
                handle_error("Error creating the extracted directory");
            }
        }
        else
        {
            int remainingSize = octal_to_int(fileHeader->size);
            extract_file_tar(archive, outputPath, remainingSize);
        }
    }

    fclose(archive);
}

// FMO02 - Decompression de l'archive compresse (tar.gz)
// Function to extract a file from the compressed archive (tar.gz)
void extract_file_tar_gz(gzFile archive, const char *outputPath, int fileSize)
{
    // Open the output file
    FILE *outputFile = fopen(outputPath, "wb");
    if (outputFile == NULL)
    {
        handle_error("Erreur lors de la création du fichier extrait");
    }

    // Read the file content from the archive and write it to the output file
    while (fileSize > 0)
    {
        // Read the file content from the archive
        int readSize = gzread(archive, buffer, sizeof(buffer));
        if (readSize <= 0)
        {
            handle_error("Erreur lors de la lecture du fichier compressé");
        }

        int writeSize = fwrite(buffer, 1, readSize, outputFile);
        if (writeSize != readSize)
        {
            handle_error("Erreur lors de l'écriture du fichier extrait");
        }

        fileSize -= readSize;
    }

    fclose(outputFile);
}

// FMO02 - Decompression de l'archive compresse (tar.gz)
// Function to extract the compressed archive (tar.gz)
void extract_archive_tar_gz(const char *archivePath, const char *outputDir)
{
    // Open the archive
    gzFile archive = open_archive(archivePath);

    // Create the output directory
    if (!create_directory(outputDir))
    {
        handle_error("Erreur lors de la création du répertoire de sortie");
    }

    // Read the archive header by header
    while (gzread(archive, buffer, BLOCK_SIZE) > 0)
    {
        // Get the header from the buffer
        struct header_tar *fileHeader = (struct header_tar *)buffer;

        // Check if it's the end of the archive
        if (is_end_of_archive(fileHeader->name))
        {
            break; // End of the archive
        }

        // Get the file size from the header
        char outputPath[PATH_MAX];
        // Create the output path
        snprintf(outputPath, sizeof(outputPath), "%s/%s", outputDir, fileHeader->name);

        // Extract the file or create the directory
        if (fileHeader->typeflag == '5')
        {
            if (!create_directory(outputPath))
            {
                handle_error("Erreur lors de la création du répertoire extrait");
            }
        }
        else
        {
            int remainingSize = octal_to_int(fileHeader->size);
            extract_file_tar_gz(archive, outputPath, remainingSize);
        }
    }

    gzclose(archive);
}

// FM03 - Creation d'une archive (tar)
void archive_file_tar(FILE *archive, const char *filepath) {
    FILE *input_file = fopen(filepath, "rb");
    check_file_open_error(input_file, filepath);

    write_header_tar(archive, filepath, 0);

    int read_size;
    char buffer[BLOCK_SIZE];

    while ((read_size = fread(buffer, 1, sizeof(buffer), input_file)) > 0) {
        fwrite(buffer, 1, read_size, archive);
    }

    fclose(input_file);

    int padding_size = BLOCK_SIZE - (get_file_size(filepath) % BLOCK_SIZE);
    if (padding_size < BLOCK_SIZE) {
        add_padding_tar(archive, padding_size);
    }
}

// FM03 - Creation d'une archive (tar)
void archive_directory_tar(FILE *archive, const char *dirpath) {
    DIR *dir = opendir(dirpath);
    check_file_open_error(dir, dirpath);

    struct dirent *entry;
    char entry_path[PATH_MAX];
    write_header_tar(archive, dirpath, 1);

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            snprintf(entry_path, sizeof(entry_path), "%s/%s", dirpath, entry->d_name);

            struct stat st;
            if (stat(entry_path, &st) == 0) {
                if (S_ISREG(st.st_mode)) {
                    archive_file_tar(archive, entry_path);
                } else if (S_ISDIR(st.st_mode)) {
                    archive_directory_tar(archive, entry_path);
                }
                // You can add more conditions to handle other file types if needed
            }
        }
    }

    closedir(dir);
}

// FM03 - Creation d'une archive (tar)
void create_archive_tar(const char *output_archive, const char *input_files[], int num_files) {
    FILE *archive = fopen(output_archive, "wb");
    check_file_open_error(archive, output_archive);

    char buffer[BLOCK_SIZE];

    for (int i = 0; i < num_files; ++i) {
        struct stat st;
        if (stat(input_files[i], &st) == 0) {
            if (S_ISREG(st.st_mode)) {
                archive_file_tar(archive, input_files[i]);
            } else if (S_ISDIR(st.st_mode)) {
                archive_directory_tar(archive, input_files[i]);
            }
            // You can add more conditions to handle other file types if needed
        }
    }

    memset(buffer, 0, sizeof(buffer));
    add_padding_tar(archive, BLOCK_SIZE);
    add_padding_tar(archive, BLOCK_SIZE);

    fclose(archive);
}

// FMO01 - Creation d'une archive compresse (tar.gz)
void archive_file_tar_gz(gzFile archive, const char *filepath)
{
    FILE *input_file = fopen(filepath, "rb");
    check_file_open_error(input_file, filepath);

    write_header(archive, filepath, 0);

    int read_size;
    char buffer[BLOCK_SIZE];

    while ((read_size = fread(buffer, 1, sizeof(buffer), input_file)) > 0)
    {
        gzwrite(archive, buffer, read_size);
    }

    fclose(input_file);

    int padding_size = BLOCK_SIZE - (get_file_size(filepath) % BLOCK_SIZE);
    if (padding_size < BLOCK_SIZE)
    {
        add_padding(archive, padding_size);
    }
}

// FMO01 - Creation d'une archive compresse (tar.gz)
void archive_directory_tar_gz(gzFile archive, const char *dirpath)
{
    DIR *dir = opendir(dirpath);
    check_file_open_error(dir, dirpath);

    struct dirent *entry;
    char entry_path[PATH_MAX];
    write_header(archive, dirpath, 1);

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            snprintf(entry_path, sizeof(entry_path), "%s/%s", dirpath, entry->d_name);

            struct stat st;
            if (stat(entry_path, &st) == 0)
            {
                if (S_ISREG(st.st_mode))
                {
                    archive_file_tar_gz(archive, entry_path);
                }
                else if (S_ISDIR(st.st_mode))
                {
                    archive_directory_tar_gz(archive, entry_path);
                }
                // You can add more conditions to handle other file types if needed
            }
        }
    }

    closedir(dir);
}

// FMO01 - Creation d'une archive compresse (tar.gz)
void create_archive_tar_gz(const char *output_archive, const char *input_files[], int num_files)
{
    gzFile archive = gzopen(output_archive, "wb");
    check_file_open_error(archive, output_archive);

    char buffer[BLOCK_SIZE];

    for (int i = 0; i < num_files; ++i)
    {
        struct stat st;
        if (stat(input_files[i], &st) == 0)
        {
            if (S_ISREG(st.st_mode))
            {
                archive_file_tar_gz(archive, input_files[i]);
            }
            else if (S_ISDIR(st.st_mode))
            {
                archive_directory_tar_gz(archive, input_files[i]);
            }
            // You can add more conditions to handle other file types if needed
        }
    }

    memset(buffer, 0, sizeof(buffer));
    add_padding(archive, BLOCK_SIZE);
    add_padding(archive, BLOCK_SIZE);

    gzclose(archive);
}

