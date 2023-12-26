// utils.h

#ifndef UTILS_H
#define UTILS_H

#include <errno.h>
#include <sys/stat.h>
#include <stddef.h>
#include <zlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int create_directory(const char *path);
int get_file_size(const char *file_path);
int octal_to_int(const char *octal_str);

#endif