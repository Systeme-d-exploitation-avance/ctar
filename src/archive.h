// archive.h

#ifndef ARCHIVE_H
#define ARCHIVE_H

#include "typedef.h"

// Déclaration des fonctions
void list_files(const char *archive_path);
void extract_archive(const char *archive_path);
void create_archive(const char *output_archive, const char *input_files[], int num_files);

#endif
