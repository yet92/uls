//
// Created by zyzy9_0j on 23.12.2021.
//
#include "multiply_columns.h"

void print_tab(int max_len, char *name) {
    int tab_number = 0;
    int diff = max_len - mx_strlen(name);
    tab_number = diff / TAB_SIZE;
    if (diff % TAB_SIZE != 0) tab_number++;
    for (int tab_index = 0; tab_index < tab_number; tab_index++) mx_printchar('\t');
}

void set_up_multiply_columns_data(int *column_width, int *columns_number, int *rows_number, struct dirent** dirents, int dirents_length) {

    int max_file_name_length = calculate_max_name_length(dirents, dirents_length);

    struct winsize ts;
    ioctl(0, TIOCGWINSZ, &ts);
    int term_width = ts.ws_col;

    *column_width = max_file_name_length;

    *column_width = (TAB_SIZE) - (max_file_name_length % (TAB_SIZE)) + max_file_name_length;


    *columns_number = term_width / *column_width;

    // if (*column_width * *columns_number > term_width && *columns_number > 1) (*columns_number)--;
    
    *rows_number = dirents_length / *columns_number;

    if (*rows_number == 0 || dirents_length % *columns_number != 0) (*rows_number)++;
}

void set_up_multiply_columns_data_for_files(int *column_width, int *columns_number, int *rows_number, char** files, int files_number) {
    int max_file_name_length = 0;
    for (int file_index = 0; file_index < files_number; file_index++) {
        int length = mx_strlen(files[file_index]);
        if (length > max_file_name_length) {
            max_file_name_length = length;
        }
    }

    struct winsize ts;
    ioctl(0, TIOCGWINSZ, &ts);
    int term_width = ts.ws_col;

    *column_width = max_file_name_length;

    *column_width = (TAB_SIZE) - (max_file_name_length % (TAB_SIZE)) + max_file_name_length;


    *columns_number = term_width / *column_width;

    // if (*column_width * *columns_number > term_width && *columns_number > 1) (*columns_number)--;
    
    *rows_number = files_number / *columns_number;

    if (*rows_number == 0 || files_number % *columns_number != 0) (*rows_number)++;
}

void multiply_columns_print(char *path) {

    int length = 0;
    DIR* dir = NULL;
    struct dirent** dirents = generate_dirent_array(path, &length, &dir);

    int column_width;
    int columns_number;
    int rows_number;

    mx_quicksort_dirent(dirents, 0, length - 1);
    set_up_multiply_columns_data(&column_width, &columns_number, &rows_number, dirents, length);

    for (int row = 0; row < rows_number; row++) {
        for (int column = 0; column < columns_number; column++) {
            int index = row + (column * rows_number);
            // mx_printint(index);
            if (index >= length) continue;

            if(is_directory(dirents[index]->d_name)) {
                // mx_printstr(DRCT_CLR);
                mx_printstr(dirents[index]->d_name);
                // mx_printstr(DFLT_CLR); 
            } else {
                mx_printstr(dirents[index]->d_name);
            }
                
            
            if (index + rows_number < length)
                print_tab(column_width, dirents[index]->d_name);

            
        }
        mx_printchar('\n');
    }
    free(dirents);
    closedir(dir);

}

void multiply_columns_files_print(char** pathes_to_files, int pathes_to_files_len) {
    int column_width;
    int columns_number;
    int rows_number;

    int need_new_line = 0;

    set_up_multiply_columns_data_for_files(&column_width, &columns_number, &rows_number, pathes_to_files, pathes_to_files_len);
    sort_args(pathes_to_files, 0, pathes_to_files_len - 1);

    for (int row = 0; row < rows_number; row++) {
        for (int column = 0; column < columns_number; column++) {
            int index = row + (column * rows_number);

            if (index >= pathes_to_files_len) continue;

            mx_printstr(pathes_to_files[index]);
            need_new_line = 1;

            if (index + rows_number < pathes_to_files_len)
                print_tab(column_width, pathes_to_files[index]);
        }
        if (need_new_line) mx_printchar('\n');
    }

}
