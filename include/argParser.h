/// \file
/// \brief Parses the arguments passed to the program.

#ifndef ARG_PARSER_H
#define ARG_PARSER_H

/**
 * @brief Parses the arguments passed to the program.
 * 
 * This function parses the arguments passed to the program and do the corresponding actions.
 * 
 * @param argc The number of arguments.
 * @param argv The arguments.
 * 
 * @return void
 * 
*/
void parse_arguments(int argc, char *argv[]);

/**
 * @brief Prints the help.
 * 
 * This function prints the help.
 * 
 * @return void
 * 
*/
void print_help();

#endif // ARG_PARSER_H