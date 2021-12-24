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
    *rows_number = dirents_length / *columns_number;

    if ((dirents_length % *columns_number) && (*columns_number * (*rows_number) < dirents_length)) (*rows_number)++;

}

void multiply_columns_print(char *path) {

    int length = 0;
    struct dirent** dirents = generate_dirent_array(path, &length);

    int column_width;
    int columns_number;
    int rows_number;

    set_up_multiply_columns_data(&column_width, &columns_number, &rows_number, dirents, length);

    for (int row = 0; row < rows_number; row++) {
        for (int column = 0; column < columns_number; column++) {
            int index = row + (column * rows_number);
            // mx_printint(index);
            if (index >= length) continue;

            if(opendir(dirents[index]->d_name)) {
                mx_printstr(DRCT_CLR);
                mx_printstr(dirents[index]->d_name);
                mx_printstr(DFLT_CLR); 
            } else {
                mx_printstr(dirents[index]->d_name);
            }
                
            
            if (index + rows_number < length)
                print_tab(column_width, dirents[index]->d_name);

            
        }
        mx_printchar('\n');
    }


}
