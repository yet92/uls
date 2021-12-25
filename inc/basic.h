
#ifndef ULS_BASIC_H
#define ULS_BASIC_H

#include "libmx.h" 
#include <dirent.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <time.h> 
#include <errno.h> 
#include <time.h> 
#include <grp.h> 
#include <pwd.h> 
#include <sys/xattr.h> 
#include <sys/acl.h> 
#include <sys/ioctl.h>

void basic_ls(char *path);
bool check_name(char *);

int calculate_max_name_length(struct dirent *dirents[], int dirents_size);
struct dirent** generate_dirent_array(char *path, int *length, DIR** to_close_dir);

void sort_args(char **args, int left, int right);
bool is_file_exist(char* path);
bool is_directory(char *name);
int args_comparator(char* name1, char* name2);

#endif //ULS_BASIC_H
