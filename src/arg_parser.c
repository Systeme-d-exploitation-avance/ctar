#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "../include/archive.h"
#include "../include/arg_parser.h"

void print_help()
{
    printf("Usage: program [OPTIONS] [ARCHIVE_FILE]\n");
    printf("Options:\n");
    printf("  -l, --list        ARCHIVE_FILE            List contents of the archive\n");
    printf("  -e, --extract     ARCHIVE_FILE            Extract the archive\n");
    printf("  -c, --create      ARCHIVE_FILE            Create a new archive\n");
    printf("  -d, --directory   DIRECTORY_TO_PROCESS    Specify the directory to process (used with -c and -e)\n");
    printf("  -z, --compress                            Compress the archive (used with -c : exemple ./program -zc my.tar fileA)\n");
    printf("  -v, --verbose                             Enable verbose mode\n");
    printf("  -h, --help                                Display this help\n");
    exit(EXIT_SUCCESS);
}

void parse_arguments(int argc, char *argv[])
{
    const struct option long_options[] = {
        {"list", required_argument, 0, 'l'},
        {"extract", required_argument, 0, 'e'},
        {"create", required_argument, 0, 'c'},
        {"directory", required_argument, 0, 'd'},
        {"compress", no_argument, 0, 'z'},
        {"verbose", no_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}};

    int opt;
    char *archive_path = NULL;
    char *output_archive = NULL;
    char *output_directory = ".";
    int verbose_mode = 0;
    int compress_flag = 0;

    while ((opt = getopt_long(argc, argv, "l:e:c:d:zvh", long_options, NULL)) != -1)
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
                optind++;
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

            // Check if the '-z' flag is present
            if (compress_flag)
            {
                size_t compressed_len = strlen(output_archive) + strlen(".gz") + 1; // +1 for null terminator
                char *output_tar_gz = malloc(compressed_len);
                snprintf(output_tar_gz, compressed_len, "%s.gz", output_archive);
                printf("Compressing archive %s to %s.gz\n", output_archive, output_archive);
                compress_tar_to_gz(output_archive, output_tar_gz);
                remove(output_archive);
            }
            break;

        case 'z':
            // Set a flag indicating that compression is requested
            printf("compress_flag set to 1\n");
            compress_flag = 1;
            break;

        case 'd':
            if (optarg == NULL)
            {
                handle_error("Error: No directory specified.");
            }

            output_directory = optarg;
            break;

        case 'v':
            verbose_mode = 1;
            break;

        case 'h':
            print_help();
            break;

        default:
            handle_error("Error: Invalid arguments.");
        }
    }
}
