#ifndef ULS_L_FLAG_H
#define ULS_L_FLAG_H

#include "libmx.h"
#include "flags.h"


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

t_lf_info* create_lf_info() {
    t_lf_info* info = (t_lf_info *)malloc(sizeof(t_lf_info));
    info->len_rights = 0;
    info->len_links = 0;
    info->len_user = 0;
    info->len_group = 0;
    info->len_size = 0;
    info->total = 0;
    return info;
}

void set_lf_info(t_lf_info** info, struct dirent** dirents, int length);


#endif
