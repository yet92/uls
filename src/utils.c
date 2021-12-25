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

int mx_quicksort_dirent(struct dirent **dirents, int left, int right) {

    int i, j;
    i = left;
    j = right;

    struct dirent *tmp, *anchor;
    anchor = dirents[left + (right - left) / 2];
    char* anchor_name = anchor->d_name;

    int swaps = 0;

    char* i_name = NULL;
    char* j_name = NULL;

    do {

        i_name = dirents[i]->d_name;
        j_name = dirents[j]->d_name;


        // while (i_len < anchor_len) {
        while(mx_strcmp(anchor_name, i_name) > 0)
        {
            i++;
            i_name = dirents[i]->d_name;
        }
        // while (j_len > anchor_len) {
        while(mx_strcmp(anchor_name, j_name) < 0)
        {
            j--;
            j_name = dirents[j]->d_name;
        }
        if (i <= j) {
            if (mx_strcmp(i_name, j_name) > 0) {
                tmp = dirents[i];
                dirents[i] = dirents[j];
                dirents[j] = tmp;
                swaps++;
            }
            i++;
            if (j > 0) j--;
        }
    } while (i <= j);

    if (i < right) {
        swaps += mx_quicksort_dirent(dirents, i, right);
    }
    if (j > left) {
        swaps += mx_quicksort_dirent(dirents, left, j);
    }
    
    return swaps;
}
