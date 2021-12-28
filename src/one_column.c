#include "one_column.h"

void one_column_print(char *path) {

    int length = 0;
    DIR* dir = NULL;
    struct dirent** dirents = generate_dirent_array(path, &length, &dir);

    mx_quicksort_dirent(dirents, 0, length - 1);

    for (int dirent_index = 0; dirent_index < length; dirent_index++) {
        mx_printstr(dirents[dirent_index]->d_name);
        mx_printchar('\n');
    }

}

void one_column_files_print(char** pathes_to_files, int pathes_to_files_len) {
    if (pathes_to_files_len == 0) return;
    sort_args(pathes_to_files, 0,pathes_to_files_len - 1);
    for (int file_index = 0; file_index < pathes_to_files_len; file_index++) {
        mx_printstr(pathes_to_files[file_index]);
        mx_printchar('\n');
    }
}
