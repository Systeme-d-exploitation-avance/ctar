// utils.h

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

int create_directory(const char *path);
int get_file_size(const char *file_path);
int octal_to_int(const char *octal_str);
void check_file_open_error(void* file, const char *filePath);
void handle_error(const char *message);
gzFile open_archive(const char *archive_path);
bool is_end_of_archive(const char *name);
void calculate_checksum(struct header_tar *header);
void add_padding_gz(gzFile archive, int size);
void add_padding(FILE *archive, int size);
void write_header_gz(gzFile archive, const char *filename, int is_directory);
void write_header(FILE *archive, const char *filename, int is_directory);
void skip_to_next_header(gzFile archive, size_t fileSize);

#endif