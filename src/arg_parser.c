// arg_parser.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/archive.h"
#include "../include/arg_parser.h"

void parse_arguments(int argc, char *argv[])
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
                handle_error("Error: No archive path specified.");
            }

            archive_path = argv[optind];
            optind++;

            if (opt == 'x' && optind < argc)
            {
                output_directory = argv[optind];
            }

            if (opt == 'l')
            {
                printf("Listing archive %s :\n", archive_path);
                list_files(archive_path);
            }
            else if (opt == 'x')
            {
                printf("Extracting archive %s to %s\n", archive_path, output_directory);
                extract_archive(archive_path, output_directory);
                printf("Done!\n");
            }
            break;

        case 'c':
            if (optarg == NULL)
            {
                handle_error("Error: No archive name specified.");
            }

            output_archive = optarg;
            const char **input_files = (const char **)&argv[optind];
            int num_files = argc - optind;

            printf("Archive Name: %s\n", output_archive);
            printf("File/Folder to archive list:\n");
            for (int i = 0; i < num_files; ++i)
            {
                printf("   %s\n", input_files[i]);
            }

            create_archive(output_archive, input_files, num_files);
            break;

        case 'z':
            if (optarg == NULL)
            {
                handle_error("Error: No archive name specified.");
            }

            archive_path = optarg;

            if (optind >= argc)
            {
                handle_error("Error: No output archive name specified.");
            }

            output_archive = argv[optind];
            optind++;

            compress_tar_to_gz(archive_path, output_archive);
            break;

        default:
            handle_error("Error: Invalid arguments.");
        }
    }
}