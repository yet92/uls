#include "ls_handlers.h"

t_flags *create_t_flag() {
    t_flags *flag = (t_flags*)malloc(sizeof(t_flags));
    flag->l_flag = false;
    flag->no_flag = true;
    

    return flag;
}

void check_flag(char *data, t_flags *flag, char* illegal_flag_option) {
    // if(!mx_strcmp(data, "-l"))
    //     flag->l_flag = true;
    // // write all flags
    // else 
    //     flag->wrong_flag = true;
    // TODO: swith no flag to false
    for (int data_index = 1; data_index < mx_strlen(data); data_index++) {
        switch (data[data_index])
        {
            case 'l':
                flag->l_flag = true;
                flag->no_flag = false;
                break;
            
            default:
                *illegal_flag_option = data[data_index];
                flag->wrong_flag = true;
                return;
        }
    }
}

void l_flag_without_args() {
    t_lf_info* lf_info = NULL;

    int dirents_length = 0;
    DIR* dir;
    struct dirent** dirents = generate_dirent_array(".", &dirents_length, &dir);
    char* full_path = generate_full_path(".");

    set_lf_info(&lf_info, dirents, dirents_length, full_path);

    mx_printstr("total ");
    mx_printint(lf_info->total);
    mx_printchar('\n');
    l_flag_print(full_path, lf_info);

    free(full_path);
    free(dirents);
    closedir(dir);
}

void check_args(int argc, char **argv) {
    // int file_counter = argc;
    t_flags *flag = create_t_flag();
    if(argc == 1) {
        flag->no_flag = true;
        if (isatty(1)) {
            multiply_columns_print(".");
        } else {
            one_column_print(".");
        }
        return;
    }


    int argv_shift = 0;
    if(argc >= 2) {
        if(IS_FLAG(argv[1])) {
            //check which flag
            if (mx_strlen(argv[1]) > 1) {
                char illegal_flag = ' ';
                check_flag(argv[1], flag, &illegal_flag);
                
                if (flag->wrong_flag) {
                    mx_printerr("uls: illegal option -- ");
                    mx_printerr(&illegal_flag); // TODO: errors?
                    mx_printerr("\n");
                    mx_printerr("usage: uls [-l] [file ...]\n");
                    exit(1);
                }

                if (argc == 2) {
                    l_flag_without_args();
                    return;
                }

                argv_shift = 2;
            }
        } else {
            argv_shift = 1;
        }
    }
   
    sort_args(argv + argv_shift, 0, argc - argv_shift - 1);

    // check on errors
    // print all files
    // print directories

    int exit_status = pathes_errors_print(argv + argv_shift, argc - argv_shift);

    correct_args_handler(argv + argv_shift, argc - argv_shift, flag);

    free(flag);
    exit(exit_status);
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

    // t_lf_info* lf_info = NULL;
    t_lf_info_node* lf_info_list = NULL;
    t_lf_info_node* current_lf_info = lf_info_list;
    int has_files = 0;

    for (int path_index = 0; path_index < pathes_number; path_index++) {
        if (pathes[path_index]) {
            if (is_directory(pathes[path_index])) {
                int dirents_length = 0;
                DIR* dir;
                struct dirent** dirents = generate_dirent_array(pathes[path_index], &dirents_length, &dir);
                char* full_path = generate_full_path(pathes[path_index]);

                t_lf_info_node* pushed = push_lf_info_list(&lf_info_list);

                if (current_lf_info == NULL) current_lf_info = pushed;

                set_lf_info(&(pushed->lf_info), dirents, dirents_length, full_path);
                /*
                lf_info->totals[index].index = path_index;
                lf_info->totals[index].total = lf_info->total;

                */

                // lf_info->total = 0;
                free(dirents);
                free(full_path);
                closedir(dir);
            } else {
                
                // TODO: edit if  -r
                if (lf_info_list == NULL) {
                    t_lf_info_node* pushed = push_lf_info_list(&lf_info_list);
                    current_lf_info = pushed;
                }
                set_lf_info_for_path(&(lf_info_list->lf_info), pathes[path_index]);
                has_files = 1;
                // lf_info->total = 0;
            }
        }
    }
    
    
    if (has_files) {
        files_handler(pathes, pathes_number, flags, &current_lf_info);
        current_lf_info = current_lf_info->next;
    }
    directories_handler(pathes, pathes_number, flags, &current_lf_info);

    free_lf_info_list(&lf_info_list);

    // free(lf_info);
}

bool is_nulls(char** pathes, int pathes_number) {
    
    for(int index = 0; index < pathes_number; index++) {
        if (pathes[index]) return false;
    }
    return true;
}

void files_handler(char** pathes, int pathes_number, t_flags* flags, t_lf_info_node** lf_info_current) {

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
                    char * l_flag_str = generate_lflg_string(pathes[path_index], (*lf_info_current)->lf_info, pathes[path_index]);
                    mx_printstr(l_flag_str);
                    mx_printchar('\n');
                    free(l_flag_str);
                    pathes[path_index] = NULL;
                    in_loop = 1;
                }
            }
        }
        // *lf_info_current = (*lf_info_current)->next;
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
        
        if (isatty(1)) {
            multiply_columns_files_print(files_pathes, f_pathes_index);
        } else {
            one_column_files_print(files_pathes, f_pathes_index);
        }

        free(files_pathes);
    }
    if (in_loop && !is_nulls(pathes, pathes_number)) mx_printchar('\n');

}

void directories_handler(char** pathes, int pathes_number, t_flags* flags, t_lf_info_node** lf_info_node) {
    for (int path_index = 0; path_index < pathes_number; path_index++) {
        if (pathes[path_index]) {
            if (pathes_number >= 2) {
                mx_printstr(pathes[path_index]);
                mx_printstr(":\n");
            }
            if (flags->l_flag) {
                if (get_dirents_number(pathes[path_index]) != 0)
                {
                    mx_printstr("total ");
                    // mx_printint(lf_info->current_total->total_blocks);
                    mx_printint((*lf_info_node)->lf_info->total);
                    mx_printchar('\n');


                    l_flag_print(pathes[path_index], (*lf_info_node)->lf_info);
                    (*lf_info_node) = (*lf_info_node)->next;
                }
            } else {
                
                if (get_dirents_number(pathes[path_index]) != 0) {
                    if (isatty(1)) {
                        multiply_columns_print(pathes[path_index]);
                    } else {
                        one_column_print(pathes[path_index]);
                    }
                }
            }
            pathes[path_index] = NULL;
            if (!is_nulls(pathes, pathes_number)) mx_printchar('\n');
        }
        
    }
}
