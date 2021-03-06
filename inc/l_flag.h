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
    int total;
}              t_lf_info;


typedef struct s_lf_info_node {
    t_lf_info* lf_info;
    struct s_lf_info_node* next;
}              t_lf_info_node;



t_lf_info* create_lf_info();
t_lf_info_node* create_lf_info_node();

t_lf_info_node* push_lf_info_list(t_lf_info_node** head);

void free_lf_info_list(t_lf_info_node** head);

void set_lf_info(t_lf_info** info, struct dirent** dirents, int length, char *path);
int get_lf_table_width(t_lf_info *info);

void l_flag_print(char *path, t_lf_info* lf_info);
char *generate_lflg_string(char *path, t_lf_info *info, char *name);
void set_lf_info_for_path(t_lf_info** info, char* full_path);



#endif
