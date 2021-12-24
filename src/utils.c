#include "utils.h"

char *mx_strnew_with(int size, char default_symbol) {
    char *str = mx_strnew(size);
    for (int i = 0; i < size; i++) str[i] = default_symbol;
    return str;
}

char *mx_strcpy_inf(char *dst, const char *src) {
    int i = 0;
    for (; src[i] != '\0'; i++) {
        dst[i] = src[i];
    }
    
    // dst[i] = '\0'; 

    return dst;
}

