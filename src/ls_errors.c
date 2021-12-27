#include "ls_errors.h"

char *get_path_error(char *path) {

    char* error_message = NULL;
    
    int path_len = mx_strlen(path);
    char* path_without_slash = mx_strdup(path);
    if (path[path_len - 1] == '/') path_without_slash[path_len - 1] = '\0';

    if (!is_file_exist(path_without_slash)) {
        error_message = mx_strnew(0);
        error_message = mx_strjoin_nleak(error_message, "uls: ");
        error_message = mx_strjoin_nleak(error_message, path);
        error_message = mx_strjoin_nleak(error_message, ": No such file or directory\n");
    }
    else if (path[path_len - 1] == '/') {
        if (opendir(path) == NULL) {
            error_message = mx_strnew(0);
            error_message = mx_strjoin_nleak(error_message, "uls: ");
            error_message = mx_strjoin_nleak(error_message, path);
            error_message = mx_strjoin_nleak(error_message, ": Not a directory\n");
        }
    }

    mx_strdel(&path_without_slash);
    return error_message;
}

int pathes_errors_print(char** pathes, int pathes_number) {
    int exit_status = 0;
    for (int path_index = 0; path_index < pathes_number; path_index++) {
        char * error_message = get_path_error(pathes[path_index]);

        if (error_message != NULL) {
            exit_status = 1;
            mx_printerr(error_message);
            pathes[path_index] = NULL;
            mx_strdel(&error_message);
        }
    }
    return exit_status;
}
