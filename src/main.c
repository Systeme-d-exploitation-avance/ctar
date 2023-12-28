/// \file
/// \brief The entry point of the program.
///
/// This file contains the main function of the program. It parses command-line
/// arguments using the `parse_arguments` function and returns EXIT_SUCCESS.
///

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/archive.h"
#include "../include/argParser.h"

/**
 * @brief The main function of the program.
 *
 * This function is the entry point of the program. It parses command-line
 * arguments using the `parse_arguments` function and returns EXIT_SUCCESS.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of strings representing the command-line arguments.
 * @return EXIT_SUCCESS if the program executed successfully.
 */
int main(int argc, char *argv[])
{
    parse_arguments(argc, argv);
    return EXIT_SUCCESS;
}
