/// \file
/// \brief Header file for archive-related functions.
///
/// This file defines functions for listing files, extracting archives, creating archives,

#include "../include/archive.h"

char buffer[BLOCK_SIZE];

// FM01 - Listage des fichiers de l'archive (tar et gz)
// Function to list the files in the archive
void list_files(const char *archivePath) {
    gzFile archive = gzopen(archivePath, "rb");
    check_file_open_error(archive, archivePath);

    // Read the tar archive header by header
    while (gzread(archive, buffer, BLOCK_SIZE) > 0) {
        // Interpret the buffer as a tar header structure
        struct header_tar *fileHeader = (struct header_tar *)buffer;

        // Check if the current header marks the end of the archive
        if (is_end_of_archive(fileHeader->name)) {
            break;
        }

        // Print the name of the file from the header
        printf("%s\n", fileHeader->name);

        // Extract the file size from the header and move to the next header position
        size_t fileSize = strtol(fileHeader->size, NULL, 8);
        skip_to_next_header(archive, fileSize);
    }

    gzclose(archive);
}

// FM02 & FMO02 - Extraction de l'archive (tar et gz)
// Function to extract the archive
void extract_archive(const char *archivePath, const char *outputDir)
{
    // Check if the file has a ".gz" extension and call the corresponding function
    size_t len = strlen(archivePath);
    if (len >= 3 && strcmp(archivePath + len - 3, ".gz") == 0)
    {
        printf("Extracting a compressed archive\n");
        extract_archive_tar_gz(archivePath, outputDir);
    }
    else
    {
        printf("Extracting a non-compressed archive\n");
        extract_archive_tar(archivePath, outputDir);
    }
}

// FM02 - Extraction de l'archive (tar)
// Function to extract a file from the archive (tar)
void extract_file_tar(FILE *archive, const char *outputPath, int fileSize)
{
    // Open the output file
    FILE *outputFile = fopen(outputPath, "wb");
    check_file_open_error(outputFile, outputPath);

    // Read the file content from the archive and write it to the output file
    while (fileSize > 0)
    {
        // Read the file content from the archive
        int readSize = fread(buffer, 1, sizeof(buffer), archive);
        if (readSize <= 0)
        {
            handle_error("Error reading the compressed file");
        }

        // Write the file content to the output file
        int writeSize = fwrite(buffer, 1, readSize, outputFile);
        if (writeSize != readSize)
        {
            handle_error("Error writing the extracted file");
        }

        // Update the remaining file size
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
    check_file_open_error(outputFile, outputPath);

    // Read the file content from the archive and write it to the output file
    while (fileSize > 0)
    {
        // Read the file content from the archive
        int readSize = gzread(archive, buffer, sizeof(buffer));
        if (readSize <= 0)
        {
            handle_error("Erreur lors de la lecture du fichier compressé");
        }

        // Write the file content to the output file
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

// FM03 & FMO03 - Creation d'une archive (tar et gz)
// Function to create an archive
void create_archive(const char *outputArchive, const char *inputFiles[], int numFiles)
{
    // Check if the file has a ".gz" extension and call the corresponding function
    size_t len = strlen(outputArchive);
    if (len >= 3 && strcmp(outputArchive + len - 3, ".gz") == 0)
    {
        printf("Creating a compressed archive\n");
        create_archive_tar_gz(outputArchive, inputFiles, numFiles);
    }
    else
    {
        printf("Creating a non-compressed archive\n");
        create_archive_tar(outputArchive, inputFiles, numFiles);
    }
}

// FM03 - Creation d'une archive (tar)
// Function to archive a file (tar)
void archive_file_tar(FILE *archive, const char *filepath)
{
    // Open the input file
    FILE *input_file = fopen(filepath, "rb");
    check_file_open_error(input_file, filepath);

    // Write the header of the file to the archive
    write_header(archive, filepath, 0);

    int read_size;
    char buffer[BLOCK_SIZE];

    // Read the file content and write it to the archive
    while ((read_size = fread(buffer, 1, sizeof(buffer), input_file)) > 0)
    {
        fwrite(buffer, 1, read_size, archive);
    }

    fclose(input_file);

    // Add padding to reach the next block
    int padding_size = BLOCK_SIZE - (get_file_size(filepath) % BLOCK_SIZE);
    if (padding_size < BLOCK_SIZE)
    {
        add_padding(archive, padding_size);
    }
}

// FM03 - Creation d'une archive (tar)
// Function to archive a directory (tar)
void archive_directory_tar(FILE *archive, const char *dirpath)
{
    // Open the directory
    DIR *dir = opendir(dirpath);
    check_file_open_error(dir, dirpath);

    // Read the directory content
    struct dirent *entry;
    char entry_path[PATH_MAX];
    // Write the header of the directory to the archive
    write_header(archive, dirpath, 1);

    // Read the directory content and archive each file
    while ((entry = readdir(dir)) != NULL)
    {
        // Ignore the "." and ".." entries
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            // Create the path of the entry
            snprintf(entry_path, sizeof(entry_path), "%s/%s", dirpath, entry->d_name);

            // Get the file information
            struct stat st;
            // Check if the file exists
            if (stat(entry_path, &st) == 0)
            {
                // Archive the file or the directory
                if (S_ISREG(st.st_mode))
                {
                    archive_file_tar(archive, entry_path);
                }
                else if (S_ISDIR(st.st_mode))
                {
                    archive_directory_tar(archive, entry_path);
                }
            }
        }
    }

    closedir(dir);
}

// FM03 - Creation d'une archive (tar)
// Function to create an archive (tar)
void create_archive_tar(const char *output_archive, const char *input_files[], int num_files)
{
    // Open the archive
    FILE *archive = fopen(output_archive, "wb");
    check_file_open_error(archive, output_archive);

    char buffer[BLOCK_SIZE];

    // Archive each file
    for (int i = 0; i < num_files; ++i)
    {
        // Get the file information
        struct stat st;
        // Check if the file exists
        if (stat(input_files[i], &st) == 0)
        {
            // Archive the file or the directory
            if (S_ISREG(st.st_mode))
            {
                archive_file_tar(archive, input_files[i]);
            }
            else if (S_ISDIR(st.st_mode))
            {
                archive_directory_tar(archive, input_files[i]);
            }
            // You can add more conditions to handle other file types if needed
        }
    }

    // Add padding to complete the last block
    memset(buffer, 0, sizeof(buffer));
    add_padding(archive, BLOCK_SIZE);
    add_padding(archive, BLOCK_SIZE);

    fclose(archive);
}

// FMO01 - Prise en charge de la compression d’une archive tar
// Function to compress a tar archive to a gz archive
void compress_tar_to_gz(const char *input_tar, const char *output_tar_gz)
{
    // Open the input tar file
    FILE *input_file = fopen(input_tar, "rb");
    if (!input_file)
    {
        handle_error("Error opening input tar file");
    }

    // Open the output tar.gz file
    gzFile output_gz = gzopen(output_tar_gz, "wb");
    if (!output_gz)
    {
        fclose(input_file);
        handle_error("Error opening output tar.gz file");
    }

    // Read the tar file content by chunks
    unsigned char in[CHUNK_SIZE];
    int read_size;

    // Read the tar file content and write it to the gz file
    while ((read_size = fread(in, 1, sizeof(in), input_file)) > 0)
    {
        // Write the chunk to the gz file until the end of the tar file
        if (gzwrite(output_gz, in, (unsigned)read_size) == 0)
        {
            fclose(input_file);
            gzclose(output_gz);
            handle_error("Error writing compressed file");
        }
    }

    fclose(input_file);
    gzclose(output_gz);
}

// FMO01 - Creation d'une archive compresse (tar.gz)
// Function to archive a file (tar.gz)
void archive_file_tar_gz(gzFile archive, const char *filepath)
{
    // Open the input file
    FILE *input_file = fopen(filepath, "rb");
    check_file_open_error(input_file, filepath);

    // Write the header of the file to the archive
    write_header_gz(archive, filepath, 0);

    int read_size;
    char buffer[BLOCK_SIZE];

    // Read the file content and write it to the archive
    while ((read_size = fread(buffer, 1, sizeof(buffer), input_file)) > 0)
    {
        gzwrite(archive, buffer, read_size);
    }

    fclose(input_file);

    // Add padding to reach the next block
    int padding_size = BLOCK_SIZE - (get_file_size(filepath) % BLOCK_SIZE);
    if (padding_size < BLOCK_SIZE)
    {
        add_padding_gz(archive, padding_size);
    }
}

// FMO01 - Creation d'une archive compresse (tar.gz)
// Function to archive a directory (tar.gz)
void archive_directory_tar_gz(gzFile archive, const char *dirpath)
{
    // Open the directory
    DIR *dir = opendir(dirpath);
    check_file_open_error(dir, dirpath);

    // Read the directory content
    struct dirent *entry;
    char entry_path[PATH_MAX];
    // Write the header of the directory to the archive
    write_header_gz(archive, dirpath, 1);

    // Read the directory content and archive each file
    while ((entry = readdir(dir)) != NULL)
    {
        // Ignore the "." and ".." entries
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            // Create the path of the entry
            snprintf(entry_path, sizeof(entry_path), "%s/%s", dirpath, entry->d_name);

            // Get the file information
            struct stat st;
            // Check if the file exists
            if (stat(entry_path, &st) == 0)
            {
                // Archive the file or the directory
                if (S_ISREG(st.st_mode))
                {
                    archive_file_tar_gz(archive, entry_path);
                }
                else if (S_ISDIR(st.st_mode))
                {
                    archive_directory_tar_gz(archive, entry_path);
                }
            }
        }
    }

    closedir(dir);
}

// FMO01 - Creation d'une archive compresse (tar.gz)
// Function to create an archive (tar.gz)
void create_archive_tar_gz(const char *output_archive, const char *input_files[], int num_files)
{
    // Open the archive
    gzFile archive = gzopen(output_archive, "wb");
    check_file_open_error(archive, output_archive);

    char buffer[BLOCK_SIZE];

    // Archive each file
    for (int i = 0; i < num_files; ++i)
    {
        // Get the file information
        struct stat st;
        // Check if the file exists
        if (stat(input_files[i], &st) == 0)
        {
            // Archive the file or the directory
            if (S_ISREG(st.st_mode))
            {
                archive_file_tar_gz(archive, input_files[i]);
            }
            else if (S_ISDIR(st.st_mode))
            {
                archive_directory_tar_gz(archive, input_files[i]);
            }
        }
    }

    // Add padding to complete the last block
    memset(buffer, 0, sizeof(buffer));
    add_padding_gz(archive, BLOCK_SIZE);
    add_padding_gz(archive, BLOCK_SIZE);

    gzclose(archive);
}

