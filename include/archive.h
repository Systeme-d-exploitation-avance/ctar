/// \file
/// \brief Header file for archive-related functions.
///
/// This file defines functions for listing files, extracting archives, creating archives,

#ifndef ARCHIVE_H
#define ARCHIVE_H

#include "utils.h"

/**
 * @brief Defines the size of chunk.
 * 
 * This macro defines the size of a chunk of data. It is used for reading and writing data to and from files.
*/
#define CHUNK_SIZE 16384

/**
 * @brief FM01 Lists the files in an archive (tar or gz).
 * 
 * This function lists the files and folders in an archive. It takes the path to the archive as a
 * parameter and prints the names of the files and folders in the archive to standard output.
 * 
 * @param archive_path The path to the archive.
 * 
 * @return void
 */
void list_files(const char *archive_path);

/**
 * @brief FM02 Extracts an archive (tar or gz).
 * 
 * This function extracts an archive. It takes the path to the archive and the path to the output
 * directory as parameters. It extracts the archive to the output directory by calling extract_archive_tar
 * or extract_archive_tar_gz after checking archivePath extension.
 * 
 * @param archive_path The path to the archive.
 * @param output_dir The path to the output directory.
 * 
 * @return void
 */
void extract_archive(const char *archivePath, const char *outputDir);

/**
 * @brief FM02 Extracts a file from a tar archive.
 * 
 * This function extracts a file from a tar archive. It takes the path to the archive, the path to the
 * output path, and the size of the file as parameters. It extracts the file to the output path.
 * 
 * @param archive_path The path to the archive.
 * @param output_path The path to the output file.
 * @param file_size The size of the file.
 * 
 * @return void
*/
void extract_file_tar(FILE *archive, const char *outputPath, int fileSize);

/**
 * @brief FM02 Extracts a tar archive.
 * 
 * This function extracts a tar archive. It takes the path to the archive and the path to the output
 * directory as parameters. It extracts the archive to the output directory.
 * 
 * @param archive_path The path to the archive.
 * @param output_dir The path to the output directory.
 * 
 * @return void
*/
void extract_archive_tar(const char *archivePath, const char *outputDir);

/**
 * @brief FMO02 Extracts a file from a tar.gz archive.
 * 
 * This function extracts a file from a tar.gz archive. It takes the path to the archive, the path to the
 * output file, and the size of the file as parameters. It extracts the file to the output file.
 * 
 * @param archive_path The path to the archive.
 * @param output_path The path to the output file.
 * @param file_size The size of the file.
 * 
 * @return void
 * 
*/
void extract_file_tar_gz(gzFile archive, const char *output_path, int file_size);

/**
 * @brief FMO02 Extracts a tar.gz archive.
 * 
 * This function extracts a tar.gz archive. It takes the path to the archive and the path to the output
 * directory as parameters. It extracts the archive to the output directory.
 * 
 * @param archive_path The path to the archive.
 * @param output_dir The path to the output directory.
 * 
 * @return void
 * 
*/
void extract_archive_tar_gz(const char *archive_path, const char *output_dir);

/**
 * @brief FM03 Creates an archive (tar or gz).
 * 
 * This function creates an archive. It takes the path to the archive, the paths to the files and
 * folders to archive, and the number of files and folders to archive as parameters. It creates the
 * archive by calling create_archive_tar or create_archive_tar_gz after checking output_archive extension.
 * 
 * @param output_archive The path to the archive.
 * @param input_files The paths to the files and folders to archive.
 * @param num_files The number of files and folders to archive.
 * 
 * @return void
 * 
*/
void create_archive(const char *outputArchive, const char *inputFiles[], int numFiles);

/**
 * @brief FM03 Archive a file in a tar.
 * 
 * This function archives a file in a tar. It takes the path to the archive and the path to the file
 * to archive as parameters. It archives the file to the archive.
 * 
 * @param archive The path to the archive.
 * @param filepath The path to the file to archive.
 * 
 * @return void
 * 
*/
void archive_file_tar(FILE *archive, const char *filepath);

/**
 * @brief FM03 Archive a directory in a tar.
 * 
 * This function archives a directory in a tar. It takes the path to the archive and the path to the
 * directory to archive as parameters. It archives the directory to the archive.
 * 
 * @param archive The path to the archive.
 * @param dirpath The path to the directory to archive.
 * 
 * @return void
 * 
*/
void archive_directory_tar(FILE *archive, const char *dirpath);

/**
 * @brief FM03 Creates a tar archive.
 * 
 * This function creates a tar archive. It takes the path to the archive, the paths to the files and
 * folders to archive, and the number of files and folders to archive as parameters. 
 * 
 * @param output_archive The path to the archive.
 * @param input_files The paths to the files and folders to archive.
 * @param num_files The number of files and folders to archive.
 * 
 * @return void
 * 
*/
void create_archive_tar(const char *output_archive, const char *input_files[], int num_files);

/**
 * @brief FMO01 Compresses a tar archive into a tar.gz archive.
 * 
 * This function compresses a tar archive into a tar.gz archive. It takes the path to the tar archive and the path
 * to the tar.gz archive to be created as parameters. It compresses the tar archive into the tar.gz archive.
 * 
 * @param input_tar The path to the tar archive.
 * @param output_tar_gz The path to the tar.gz archive to be created.
 * 
 * @return void
 * 
*/
void compress_tar_to_gz(const char *input_tar, const char *output_tar_gz);

/**
 * @brief FMO01 Compresses a file into a tar.gz archive.
 * 
 * This function compresses a file into a tar.gz archive. It takes the path to the archive and the path
 * to the file to compress as parameters. It compresses the file into the archive.
 * 
 * @param archive The path to the archive.
 * @param filepath The path to the file to compress.
 * 
 * @return void
 * 
*/
void archive_file_tar_gz(gzFile archive, const char* filepath);

/**
 * @brief FMO01 Compresses a directory into a tar.gz archive.
 * 
 * This function compresses a directory into a tar.gz archive. It takes the path to the archive and the path
 * to the directory to compress as parameters. It compresses the directory into the archive.
 * 
 * @param archive The path to the archive.
 * @param dirpath The path to the directory to compress.
 * 
 * @return void
 * 
 */
void archive_directory_tar_gz(gzFile archive, const char* dirpath);

/**
 * @brief FMO01 Creates a tar.gz archive.
 * 
 * This function creates a tar.gz archive. It takes the path to the archive to be created, the paths to the files and
 * folders to archive, and the number of files and folders to archive as parameters. 
 * 
 * @param output_archive The path to the archive.
 * @param input_files The paths to the files and folders to archive.
 * @param num_files The number of files and folders to archive.
 * 
 * @return void
 * 
*/
void create_archive_tar_gz(const char* output_archive, const char* input_files[], int num_files);


#endif
