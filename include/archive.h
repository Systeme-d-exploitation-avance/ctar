// archive.h

#ifndef ARCHIVE_H
#define ARCHIVE_H

#include "utils.h"

// FM01
void list_files(const char *archive_path);

// FM02
void extract_file_tar(FILE *archive, const char *outputPath, int fileSize);
void extract_archive_tar(const char *archivePath, const char *outputDir);

// FMO02
void extract_file_tar_gz(gzFile archive, const char *output_path, int file_size);
void extract_archive_tar_gz(const char *archive_path, const char *output_dir);

// FM03 and FMO03
void archive_file(gzFile archive, const char* filepath);
void archive_directory(gzFile archive, const char* dirpath);
void create_archive(const char* output_archive, const char* input_files[], int num_files);

#endif
