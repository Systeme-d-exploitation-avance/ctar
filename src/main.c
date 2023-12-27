#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/archive.h"
#include "../include/arg_parser.h"

int main(int argc, char *argv[])
{
    parse_arguments(argc, argv);
    return EXIT_SUCCESS;
}
