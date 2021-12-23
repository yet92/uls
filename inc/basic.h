
#ifndef ULS_BASIC_H
#define ULS_BASIC_H

#include "libmx.h"
#include "dirent.h"

void basic_ls(char *path);
bool check_name(char *);

int calculate_max_name_length(struct dirent *dirents[], int dirents_size);
struct dirent** generate_dirent_array(char *path, int *length);


#endif //ULS_BASIC_H
