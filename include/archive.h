// archive.h

#ifndef ARCHIVE_H
#define ARCHIVE_H

#define CHUNK_SIZE 16384

#include "utils.h"

// FM01
void list_files(const char *archive_path);

// FM02
void extract_file_tar(FILE *archive, const char *outputPath, int fileSize);
void extract_archive_tar(const char *archivePath, const char *outputDir);

// FMO02
void extract_file_tar_gz(gzFile archive, const char *output_path, int file_size);
void extract_archive_tar_gz(const char *archive_path, const char *output_dir);

// FM03
void archive_file_tar(FILE *archive, const char *filepath);
void archive_directory_tar(FILE *archive, const char *dirpath);
void create_archive_tar(const char *output_archive, const char *input_files[], int num_files);

// FMO01
void compress_tar_to_gz(const char *input_tar, const char *output_tar_gz);

// MISC
void archive_file_tar_gz(gzFile archive, const char* filepath);
void archive_directory_tar_gz(gzFile archive, const char* dirpath);
void create_archive_tar_gz(const char* output_archive, const char* input_files[], int num_files);

#endif
