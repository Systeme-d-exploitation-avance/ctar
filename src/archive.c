// archive.c

#include "../include/archive.h"

char buffer[BLOCK_SIZE];

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

// Function to extract a file from the archive
void extract_file(gzFile archive, const char *outputPath, int fileSize)
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

// Function to extract the archive
void extract_archive(const char *archivePath, const char *outputDir)
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
        if (fileHeader->name[strlen(fileHeader->name) - 1] == '/')
        {
            if (!create_directory(outputPath))
            {
                handle_error("Erreur lors de la création du répertoire extrait");
            }
        }
        else
        {
            int remainingSize = octal_to_int(fileHeader->size);
            extract_file(archive, outputPath, remainingSize);
        }
    }

    gzclose(archive);
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
