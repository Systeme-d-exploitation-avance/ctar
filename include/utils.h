/// \file
/// \brief Utility functions

#ifndef UTILS_H
#define UTILS_H

#include "typedef.h"
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stddef.h>
#include <zlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <libgen.h>
#include <limits.h>

/**
 * @brief Creates a directory if it doesn't exist.
 * 
 * This function creates a directory. It takes the path to the directory as a parameter.
 * 
 * @param path The path to the directory.
 * 
 * @return int
 * 
 */
int create_directory(const char *path);

/**
 * @brief Gets the size of a file.
 * 
 * This function gets the size of a file. It takes the path to the file as a parameter.
 * 
 * @param file_path The path to the file.
 * 
 * @return int
 * 
 */
int get_file_size(const char *file_path);

/**
 * @brief Converts an octal string to an integer.
 * 
 * This function converts an octal string to an integer. It takes the octal string as a parameter.
 * 
 * @param octal_str The octal string.
 * 
 * @return int
 * 
 */
int octal_to_int(const char *octal_str);

/**
 * @brief Checks if a file is openned correctly and handles the error if not.
 * 
 * This function checks if a file is openned correctly and handles the error if not. It takes the file and the path to the file as parameters.
 * 
 * @param file The file.
 * @param filePath The path to the file.
 * 
 * @return void
 * 
 */
void check_file_open_error(void* file, const char *filePath);

/**
 * @brief Handles an error and exits.
 * 
 * This function handles errors and exits. It takes the error message as a parameter.
 * 
 * @param message The error message.
 * 
 * @return void
 * 
 */
void handle_error(const char *message);

/**
 * @brief Opens a gzip archive.
 * 
 * This function opens a gzip archive. It takes the path to the archive as a parameter.
 * 
 * @param archive_path The path to the archive.
 * 
 * @return gzFile
 * 
 */
gzFile open_archive(const char *archive_path);

/**
 * @brief Checks if it's the end of the archive.
 * 
 * This function checks if the name field of a header is filled with zeros to check 
 * if it's the end of the archive. It takes the name field as a parameter.
 * 
 * @param name The name field.
 * 
 * @return bool
 * 
 */
bool is_end_of_archive(const char *name);

/**
 * @brief Calculates the checksum.
 * 
 * This function calculates the checksum. It takes the header as a parameter.
 * 
 * @param header The header.
 * 
 * @return void
 * 
 */
void calculate_checksum(struct header_tar *header);

/**
 * @brief Adds padding to a gzip archive.
 * 
 * This function adds padding to a gzip archive. It takes the archive and the size of the padding as parameters.
 * 
 * @param archive The archive.
 * @param size The size of the padding.
 * 
 * @return void
 * 
*/
void add_padding_gz(gzFile archive, int size);

/**
 * @brief Adds padding to a tar archive.
 * 
 * This function adds padding to a tar archive. It takes the archive and the size of the padding as parameters.
 * 
 * @param archive The archive.
 * @param size The size of the padding.
 * 
 * @return void
 * 
*/
void add_padding(FILE *archive, int size);

/**
 * @brief Writes the header of a gzip archive.
 * 
 * This function writes the header of a gzip archive. It takes the archive, the filename and a boolean to know if it's a directory as parameters.
 * 
 * @param archive The archive.
 * @param filename The filename.
 * @param is_directory A boolean to know if it's a directory.
 * 
 * @return void
 * 
*/
void write_header_gz(gzFile archive, const char *filename, int is_directory);

/**
 * @brief Writes the header of a tar archive.
 * 
 * This function writes the header of a tar archive. It takes the archive, the filename and a boolean to know if it's a directory as parameters.
 * 
 * @param archive The archive.
 * @param filename The filename.
 * @param is_directory A boolean to know if it's a directory.
 * 
 * @return void
 * 
*/
void write_header(FILE *archive, const char *filename, int is_directory);

/**
 * @brief Skips to the next header of a gzip archive.
 * 
 * This function skips to the next header of a gzip archive. It takes the archive and the size of the file as parameters.
 * 
 * @param archive The archive.
 * @param fileSize The size of the file.
 * 
 * @return void
 * 
*/
void skip_to_next_header(gzFile archive, size_t fileSize);

#endif