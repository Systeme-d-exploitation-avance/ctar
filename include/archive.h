// archive.h

#ifndef ARCHIVE_H
#define ARCHIVE_H

// Déclaration des fonctions
void list_files(const char *archive_path);
void extract_archive(const char *archive_path, const char *output_dir);
void create_archive(const char *output_archive, const char *input_files[], int num_files);

int octal_to_int(const char *octal_str);
int create_directory(const char *path);
int get_file_size(const char *file_path);

#endif
