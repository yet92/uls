#ifndef UTILS_H
#define UTILS_H
#include "libmx.h"
#include "basic.h"

char *mx_strnew_with(int size, char default_symbol);
char *mx_strcpy_inf(char *dst, const char *src);
char *mx_strjoin_nleak(char const *s1, char const *s2);
int mx_quicksort_dirent(struct dirent **dirents, int left, int right);

#endif /*UTILS_H*/
