
#include "basic.h"

int calculate_max_name_length(struct dirent *dirents[], int dirents_size) {
    int max_length = 0;
    for (int index = 0; index < dirents_size; index++) {
        int current_length = mx_strlen(dirents[index]->d_name);
        if (current_length > max_length) max_length = current_length;
    }
    return max_length;
}

struct dirent** generate_dirent_array(char *path, int *length) {
    DIR* dir = opendir(path);
    int array_length = 0;
    struct dirent* dirent = NULL;
    while ( (dirent = readdir(dir)) ) {
        if ( check_name(dirent->d_name) ) {
            array_length++;
        }
    }
    closedir(dir);

    dir = opendir(path);
    struct dirent** dirent_array = (struct dirent**)malloc(sizeof(struct dirent*) * array_length);

    int index = 0;
    while ( (dirent = readdir(dir)) ) {
        if ( check_name(dirent->d_name) ) {
            dirent_array[index] = dirent;
            index++;
        }
    }
    closedir(dir);
    *length = array_length;
    return dirent_array;
}

bool check_name(char* name) {
    if (mx_strcmp(name, ".") == 0 || mx_strcmp(name, "..") == 0 || name[0] == '.') return false;
    return true;
}

void basic_ls(char *path) {
    DIR* dir = opendir(path);
    struct dirent* dirent = NULL;

    while ( (dirent = readdir(dir)) ) {
        if ( check_name(dirent->d_name) ) {
            mx_printstr(dirent->d_name);
            mx_printstr(" ");
        }
    }
    mx_printstr("\n");

    closedir(dir);
}
