#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/archive.h"

int main(int argc, char *argv[])
{
    int opt;
    char *archive_path = NULL;
    char *output_archive = NULL;
    char *output_directory = ".";

    while ((opt = getopt(argc, argv, "lxc:z:")) != -1)
    {
        switch (opt)
        {
        case 'l':
        case 'x':
            if (optind >= argc)
            {
                fprintf(stderr, "Error: No archive path specified.\n");
                exit(EXIT_FAILURE);
            }

            archive_path = argv[optind];
            optind++;

            if (opt == 'x' && optind < argc)
            {
                output_directory = argv[optind];
            }

            if (opt == 'l')
            {
                list_files(archive_path);
            }
            else if (opt == 'x')
            {
                extract_archive_tar_gz(archive_path, output_directory);
            }
            break;

        case 'c':
            if (optarg == NULL)
            {
                fprintf(stderr, "Error: The -c option requires an archive name.\n");
                exit(EXIT_FAILURE);
            }

            output_archive = optarg;
            const char **input_files = (const char **)&argv[optind];
            int num_files = argc - optind;

            printf("Archive Name: %s\n", output_archive);
            printf("File List:\n");
            for (int i = 0; i < num_files; ++i)
            {
                printf("   %s\n", input_files[i]);
            }

            create_archive_tar(output_archive, input_files, num_files);
            break;

        case 'z':
            if (optarg == NULL)
            {
                fprintf(stderr, "Error: No source archive path specified.\n");
                exit(EXIT_FAILURE);
            }

            archive_path = optarg;

            if (optind >= argc)
            {
                fprintf(stderr, "Error: No result archive path specified.\n");
                exit(EXIT_FAILURE);
            }

            output_archive = argv[optind];
            optind++;

            compress_tar_to_gz(archive_path, output_archive);
            break;

        default:
            fprintf(stderr, "Usage: %s [-lxc archive_path]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
