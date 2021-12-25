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

char *mx_strjoin_nleak(char const *s1, char const *s2) {
    if(s1 == NULL && s2 == NULL) return NULL;
    else if(s1 == NULL) return mx_strdup(s2);
    else if(s2 == NULL) return mx_strdup(s1);
    char *result = mx_strnew(mx_strlen(s1) + mx_strlen(s2));
    result = mx_strcat(result, s1);
    result = mx_strcat(result, s2);
    char *free = (char*)s1;
    mx_strdel(&free);
    return result;
}

