// archive.h

#ifndef ARCHIVE_H
#define ARCHIVE_H

#include "utils.h"

// Déclaration des fonctions
void list_files(const char *archive_path);
void extract_file(gzFile archive, const char *output_path, int file_size);
void extract_archive(const char *archive_path, const char *output_dir);
void archive_file(gzFile archive, const char* filepath);
void archive_directory(gzFile archive, const char* dirpath);
void create_archive(const char* output_archive, const char* input_files[], int num_files);

#endif
