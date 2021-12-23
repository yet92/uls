//
// Created by zyzy9_0j on 23.12.2021.
//
#include "multiply_columns.h"

void multiply_columns_print(char *path) {

    int length = 0;
    struct dirent** dirents = generate_dirent_array(path, &length);

    int max_file_name_length = calculate_max_name_length(dirents, length);

    struct winsize ts;
    ioctl(0, TIOCGWINSZ, &ts);
    int term_width = ts.ws_col;

    int column_width = max_file_name_length;
    int columns_number = term_width / column_width;
    column_width = term_width / columns_number;
    int rows_number = length / columns_number;

    if (length % columns_number) rows_number++;
    
    mx_printint(rows_number);
    mx_printchar('\n');

    if (columns_number * rows_number < length) columns_number++;


    for (int row = 0; row < rows_number; row++) {
        for (int column = 0; column < columns_number; column++) {
            int index = row + (column * rows_number);
            // mx_printint(index);
            if (index >= length) continue;
            int name_length = mx_strlen(dirents[index]->d_name);
            mx_printstr(dirents[index]->d_name);

            for (int space_index = 0; space_index < column_width - name_length; space_index++) {
                mx_printchar(' ');
            }

            
        }
        mx_printchar('\n');
    }


}
