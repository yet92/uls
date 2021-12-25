#ifndef UTILS_H
#define UTILS_H
#include "libmx.h"

char *mx_strnew_with(int size, char default_symbol);
char *mx_strcpy_inf(char *dst, const char *src);
char *mx_strjoin_nleak(char const *s1, char const *s2);


#endif /*UTILS_H*/
