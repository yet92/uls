#ifndef ULS_L_FLAG_H
#define ULS_L_FLAG_H

#include "libmx.h"
#include "flags.h"
#include "utils.h"

typedef struct s_total {
    int total_blocks; 
    struct s_total * next;
}              t_total;

typedef struct s_lf_info {
    int len_rights;
    int len_links;
    int len_user;
    int len_group;
    int len_size;
    // int date_month;
    // int len_date_day;
    t_total *total_head;
    t_total *current_total;

    int total;
}              t_lf_info;

t_lf_info* create_lf_info();
t_total* create_total_node();

void push_total_list(t_total** head, int total);

void set_lf_info(t_lf_info** info, struct dirent** dirents, int length, char *path);
int get_lf_table_width(t_lf_info *info);

void l_flag_print(char *path, t_lf_info* lf_info);
char *generate_lflg_string(char *path, t_lf_info *info, char *name);
void set_lf_info_for_path(t_lf_info** info, char* full_path);



#endif
