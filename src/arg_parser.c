#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "../include/archive.h"
#include "../include/arg_parser.h"

void parse_arguments(int argc, char *argv[])
{
    const struct option long_options[] = {
        {"list", required_argument, 0, 'l'},
        {"extract", required_argument, 0, 'e'},
        {"create", required_argument, 0, 'c'},
        {"compress", required_argument, 0, 'z'},
        {0, 0, 0, 0}};

    int opt;
    char *archive_path = NULL;
    char *output_archive = NULL;
    char *output_directory = ".";
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, "l:e:c:z:", long_options, &option_index)) != -1)
    {
        switch (opt)
        {
        case 'l':
            if (optarg == NULL)
            {
                handle_error("Error: No archive path specified.");
            }

            archive_path = optarg;

            if (optind < argc)
            {
                output_directory = argv[optind];
                optind++; // Increment optind to move to the next argument
            }

            printf("Listing archive %s :\n", archive_path);
            list_files(archive_path);
            break;

        case 'e':
            if (optarg == NULL)
            {
                handle_error("Error: No archive path specified.");
            }

            archive_path = optarg;

            if (optind < argc)
            {
                output_directory = argv[optind];
                optind++; // Increment optind to move to the next argument
            }

            printf("Extracting archive %s to %s\n", archive_path, output_directory);
            extract_archive(archive_path, output_directory);
            printf("Done!\n");
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

            if (optind < argc)
            {
                output_archive = argv[optind];
                optind++;
            }
            else
            {
                handle_error("Error: No output archive name specified.");
            }

            compress_tar_to_gz(archive_path, output_archive);
            break;

        default:
            handle_error("Error: Invalid arguments.");
        }
    }
}
