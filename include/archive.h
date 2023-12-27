/// \file
/// \brief Header file for archive-related functions.
///
/// This file defines functions for listing files, extracting archives, creating archives,
/// and various utility functions related to file archiving.

#ifndef ARCHIVE_H
#define ARCHIVE_H

#define CHUNK_SIZE 16384

#include "utils.h"


/**
 * @brief FM01 Lists the files in an archive.
 * 
 * This function lists the files and folders in an archive. It takes the path to the archive as a
 * parameter and prints the names of the files and folders in the archive to standard output.
 * 
 * @param archive_path The path to the archive.
 */
void list_files(const char *archive_path);

// FM02

/**
 * @brief FM02 Extracts an archive (tar or gz).
 * 
 * This function extracts an archive. It takes the path to the archive and the path to the output
 * directory as parameters. It extracts the archive to the output directory.
 * 
 * @param archive_path The path to the archive.
 * @param output_dir The path to the output directory.
 */
void extract_archive(const char *archivePath, const char *outputDir);

/**
 * @brief FM02 Extracts a file from a tar archive.
 * 
 * This function extracts a file from an archive. It takes the path to the archive, the path to the
 * output file, and the size of the file as parameters. It extracts the file to the output file.
 * 
 * @param archive_path The path to the archive.
 * @param output_path The path to the output file.
 * @param file_size The size of the file.
*/
void extract_file_tar(FILE *archive, const char *outputPath, int fileSize);

/**
 * @brief FM02 Extracts a tar archive.
 * 
 * This function extracts an archive. It takes the path to the archive and the path to the output
 * directory as parameters. It extracts the archive to the output directory.
 * 
 * @param archive_path The path to the archive.
 * @param output_dir The path to the output directory.
*/
void extract_archive_tar(const char *archivePath, const char *outputDir);

// FMO02
void extract_file_tar_gz(gzFile archive, const char *output_path, int file_size);
void extract_archive_tar_gz(const char *archive_path, const char *output_dir);

// FM03
void create_archive(const char *outputArchive, const char *inputFiles[], int numFiles);

void archive_file_tar(FILE *archive, const char *filepath);
void archive_directory_tar(FILE *archive, const char *dirpath);
void create_archive_tar(const char *output_archive, const char *input_files[], int num_files);

// MISC
void archive_file_tar_gz(gzFile archive, const char* filepath);
void archive_directory_tar_gz(gzFile archive, const char* dirpath);
void create_archive_tar_gz(const char* output_archive, const char* input_files[], int num_files);

// FMO01
void compress_tar_to_gz(const char *input_tar, const char *output_tar_gz);

#endif
