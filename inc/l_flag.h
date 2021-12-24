#ifndef ULS_L_FLAG_H
#define ULS_L_FLAG_H

#include "libmx.h"
#include "flags.h"
#include "utils.h"


typedef struct s_lf_info {
    int len_rights;
    int len_links;
    int len_user;
    int len_group;
    int len_size;
    // int date_month;
    // int len_date_day;

    int total;
}              t_lf_info;

t_lf_info* create_lf_info();

void set_lf_info(t_lf_info** info, struct dirent** dirents, int length, char *path);
int get_lf_table_width(t_lf_info *info);

void l_flag_print(char *path);
char *generate_lflg_string(char *path, t_lf_info *info, char *name);



#endif
