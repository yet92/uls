
#include "basic.h"

int calculate_max_name_length(struct dirent *dirents[], int dirents_size) {
    int max_length = 0;
    for (int index = 0; index < dirents_size; index++) {
        int current_length = mx_strlen(dirents[index]->d_name);
        if (current_length > max_length) max_length = current_length;
    }
    return max_length;
}

struct dirent** generate_dirent_array(char *path, int *length, DIR** to_close_dir) {
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
    *to_close_dir = dir;
    struct dirent** dirent_array = (struct dirent**)malloc(sizeof(struct dirent*) * array_length);

    int index = 0;
    while ( (dirent = readdir(dir)) ) {
        if ( check_name(dirent->d_name) ) {
            dirent_array[index] = dirent;
            index++;
        }
    }
 
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

bool is_directory(char *name) {
    DIR *directory = NULL;
    directory = opendir(name);
    
    if(!directory) 
        return false;

    closedir(directory);
    return true;
}

int args_comparator(char* name1, char* name2) {
    bool is_name1_directory = is_directory(name1);
    bool is_name2_directory = is_directory(name2);

    if ((is_name2_directory && !is_name1_directory) || (is_name1_directory && !is_name2_directory)) 
        return  is_name1_directory - is_name2_directory;

    return mx_strcmp(name1, name2);

}

void sort_args(char **args, int left, int right) {
    int i, j;
    i = left;
    j = right;

    char *tmp, *anchor;
    anchor = args[left + (right - left) / 2];
    char* anchor_name = anchor;

    char* i_name = NULL;
    char* j_name = NULL;

    do {

        i_name = args[i];
        j_name = args[j];


        // while (i_len < anchor_len) {
        while(args_comparator(anchor_name, i_name) > 0)
        {
            i++;
            i_name = args[i];
        }
        // while (j_len > anchor_len) {
        while(args_comparator(anchor_name, j_name) < 0)
        {
            j--;
            j_name = args[j];
        }
        if (i <= j) {
            if (args_comparator(i_name, j_name) > 0) {
                tmp = args[i];
                args[i] = args[j];
                args[j] = tmp;
            }
            i++;
            if (j > 0) j--;
        }
    } while (i <= j);

    if (i < right) {
        sort_args(args, i, right);
    }
    if (j > left) {
        sort_args(args, left, j);
    }
}
