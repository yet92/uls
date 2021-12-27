#include "ls_handlers.h"

t_flags *create_t_flag() {
    t_flags *flag = (t_flags*)malloc(sizeof(t_flags));
    flag->l_flag = false;
    flag->no_flag = false;
    

    return flag;
}

void check_args(int argc, char **argv) {
    // int file_counter = argc;
    t_flags *flag = create_t_flag();
    if(argc == 1)
        flag->no_flag = true;
    

    int argv_shift = 0;
    if(argc >= 2) {
        if(IS_FLAG(argv[1])) {
            //check which flag
            flag->l_flag = true;
            argv_shift = 2;
        } else {
            flag->no_flag = true;
            argv_shift = 1;
        }
    }
   
    sort_args(argv + argv_shift, 0, argc - argv_shift - 1);

    // check on errors
    // print all files
    // print directories

    pathes_errors_print(argv + argv_shift, argc - argv_shift);

    correct_args_handler(argv + argv_shift, argc - argv_shift, flag);

    free(flag);

}

char *generate_full_path(char *path) {
    DIR *dir = NULL;
    char *full_path = NULL;
    if ((path[mx_strlen(path) - 1] != '/') && (dir = opendir(path))) 
        full_path = mx_strjoin(path, "/");
    else 
        full_path = mx_strjoin(path, NULL);
    
    if(dir) 
        closedir(dir);
    
    return full_path;
}

void correct_args_handler(char** pathes, int pathes_number, t_flags* flags) {
    // set lf_info
    // print files
    // print dirs

    t_lf_info* lf_info = NULL;

    for (int path_index = 0; path_index < pathes_number; path_index++) {
        if (pathes[path_index]) {
            if (is_directory(pathes[path_index])) {
                int dirents_length = 0;
                DIR* dir;
                struct dirent** dirents = generate_dirent_array(pathes[path_index], &dirents_length, &dir);
                char* full_path = generate_full_path(pathes[path_index]);
                set_lf_info(&lf_info, dirents, dirents_length, full_path);
                free(dirents);
                free(full_path);
                closedir(dir);
            } else {
                set_lf_info_for_path(&lf_info, pathes[path_index]);
            }
        }
    }
    

    files_handler(pathes, pathes_number, flags, lf_info);
    directories_handler(pathes, pathes_number, flags, lf_info);
    free(lf_info);
}

bool is_nulls(char** pathes, int pathes_number) {
    
    for(int index = 0; index < pathes_number; index++) {
        if (pathes[index]) return false;
    }
    return true;
}

void files_handler(char** pathes, int pathes_number, t_flags* flags, t_lf_info* lf_info) {

    int files_pathes_length = 0;
    for (int path_index = 0; path_index < pathes_number; path_index++) {
        if (pathes[path_index] != NULL) {
            if (!is_directory(pathes[path_index])) {
                files_pathes_length++;
            }
        }
    }

    int in_loop = 0;
    if (flags->l_flag) {
        for (int path_index = 0; path_index < pathes_number; path_index++) {
            if (pathes[path_index]) {
                if (!is_directory(pathes[path_index])) {
                    char * l_flag_str = generate_lflg_string(pathes[path_index], lf_info, pathes[path_index]);
                    mx_printstr(l_flag_str);
                    mx_printchar('\n');
                    free(l_flag_str);
                    pathes[path_index] = NULL;
                    in_loop = 1;
                }
            }
        }
    } else {
        char** files_pathes = (char**)malloc(sizeof(char*) * files_pathes_length);
        int f_pathes_index = 0;
        for (int path_index = 0; path_index < pathes_number; path_index++) {
            if (pathes[path_index]) {
                if (!is_directory(pathes[path_index])) {
                    files_pathes[f_pathes_index] = pathes[path_index];
                    pathes[path_index] = NULL;
                    f_pathes_index++;
                    in_loop = 1;
                }
            }
        }
        
        multiply_columns_files_print(files_pathes, f_pathes_index);

        free(files_pathes);
    }
    if (in_loop && !is_nulls(pathes, pathes_number)) mx_printchar('\n');

}

void directories_handler(char** pathes, int pathes_number, t_flags* flags, t_lf_info* lf_info) {
    for (int path_index = 0; path_index < pathes_number; path_index++) {
        if (pathes[path_index]) {
            if (pathes_number >= 2) {
                mx_printstr(pathes[path_index]);
                mx_printstr(":\n");
            }
            if (flags->l_flag) {
                l_flag_print(pathes[path_index], lf_info);
            } else {
                multiply_columns_print(pathes[path_index]);
            }
            pathes[path_index] = NULL;
            if (!is_nulls(pathes, pathes_number)) mx_printchar('\n');
        }
        
    }
}
