//
// Created by zyzy9_0j on 23.12.2021.
//

#ifndef ULS_MULTIPLY_COLUMNS_H
#define ULS_MULTIPLY_COLUMNS_H

#include "libmx.h"
#include "basic.h"
#include "dirent.h"
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include "utils.h"

#define TAB_SIZE 8
#define DFLT_CLR  "\x1B[0m"
#define DRCT_CLR "\x1B[36m"

void multiply_columns_print(char *path);
void multiply_columns_files_print(char** pathes_to_files, int pathes_to_files_len);

#endif //ULS_MULTIPLY_COLUMNS_H
