#include "l_flag.h"

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

t_lf_info_node* create_lf_info_node() {

    t_lf_info_node* node = (t_lf_info_node*)malloc(sizeof(t_lf_info_node));
    node->lf_info = create_lf_info();
    node->next = NULL;
    return node;
}

t_lf_info_node* push_lf_info_list(t_lf_info_node** head) {

    if (*head == NULL) {
        *head = create_lf_info_node();
        return *head;
    }
    else {
        t_lf_info_node* current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = create_lf_info_node();
        return current->next;
    }
    return NULL;
}

void free_lf_info_list(t_lf_info_node** head) {

    t_lf_info_node* current = *head;
    t_lf_info_node* next = NULL;

    while (current)
    {
        free(current->lf_info);
        next = current->next;
        free(current);
        current = next;
    }
    
    *head = NULL;
}

char *fmode_thirdch_check(int mode, int triad) {
    if (!(mode & triad) && (S_ISUID & mode) && triad != S_IXGRP && triad != S_IXOTH){
        return "S";
    }else if (!(mode & triad) && (S_ISVTX & mode)){
        return "T";
    }else if ((mode & triad) && (S_ISVTX & mode)  && triad != S_IXUSR && triad != S_IXGRP){
        return "t";
    }else if ((mode & triad) && (S_ISUID & mode) && triad != S_IXGRP && triad != S_IXOTH){
        return "s";
    } else if ((mode & triad) && (S_ISGID & mode) && triad != S_IXUSR && triad != S_IXOTH){
        return "s";
    }else if ((mode & triad)){
        return "x";
    } else {
        return "-";
    }
}

char *fmode_to_char(int mode, char *name) {
    char *result = mx_strnew(0);
    acl_t acl;

    if(S_ISLNK(mode)) {
        result = mx_strjoin_nleak(result, "l");
    } else if(S_ISDIR(mode)) {
        result = mx_strjoin_nleak(result, "d");
    } else if(S_ISSOCK(mode)) {
        result = mx_strjoin_nleak(result, "s");
    } else if(S_ISFIFO(mode)) {
        result = mx_strjoin_nleak(result, "p");
    } else if(S_ISBLK(mode)) {
        result = mx_strjoin_nleak(result, "b");
    } else if(S_ISCHR(mode)) {
        result = mx_strjoin_nleak(result, "c");
    } else {
        result = mx_strjoin_nleak(result, "-");
    }

    result = mx_strjoin_nleak(result, ( (mode & S_IRUSR) ? "r" : "-"));
    result = mx_strjoin_nleak(result, ( (mode & S_IWUSR) ? "w" : "-"));
    result = mx_strjoin_nleak(result, fmode_thirdch_check(mode, S_IXUSR));
    result = mx_strjoin_nleak(result, ( (mode & S_IRGRP) ? "r" : "-"));
    result = mx_strjoin_nleak(result, ( (mode & S_IWGRP) ? "w" : "-"));
    result = mx_strjoin_nleak(result, fmode_thirdch_check(mode, S_IXGRP));
    result = mx_strjoin_nleak(result, ( (mode & S_IROTH) ? "r" : "-"));
    result = mx_strjoin_nleak(result, ( (mode & S_IWOTH) ? "w" : "-"));
    result = mx_strjoin_nleak(result, fmode_thirdch_check(mode, S_IXOTH));
    
    if (listxattr(name, NULL, 0, XATTR_NOFOLLOW) > 0) {
        result = mx_strjoin_nleak(result, "@");
    } 
    if ((acl = acl_get_file(name, ACL_TYPE_EXTENDED))) {
        acl_free(acl);
        result = mx_strjoin_nleak(result, "+");
    }

    return result;
}

char *get_usr(unsigned int uid) {
    char *result_str = NULL;

    struct passwd *pws;

    pws = getpwuid(uid);

    if(pws != NULL)
        result_str = mx_strdup(pws->pw_name);
    else 
        result_str = mx_itoa(uid);
    return result_str;
}

char *get_group(unsigned int gid) {

    
    char *result_str = NULL;

    struct group *grp;

    grp = getgrgid(gid);
    
    if(grp) {
        result_str = mx_strdup(grp->gr_name);
    } else {
        result_str = mx_itoa(gid);
    }

    return result_str;
}

void turn_to_old_date(char *clock, char *result_str) {
    result_str = mx_strncpy(result_str, clock + 4, 6);
    result_str = mx_strjoin(result_str, clock + 19);
    result_str[13] = '\0';
}

char *get_fchange_date(const time_t date) {
    char *clock;
    char *result_str = mx_strnew(12);

    clock =  ctime(&(date));

    long int date_diff = time(NULL) - date;
    if(date_diff < 0) date_diff = -date_diff;

    if(date_diff < 15768000) 
        result_str = mx_strncpy(result_str, clock + 4, 12);
    else {
        result_str = mx_strncpy(result_str, clock + 4, 6);
        result_str = mx_strjoin_nleak(result_str, " ");
        result_str = mx_strjoin_nleak(result_str, clock + 19);
        result_str[12] = '\0';
    }
    return result_str;
}

char *get_link(char* path) {
    ssize_t bytes, bufsize;
    bufsize = PATH_MAX;

    char *link_to = mx_strnew(bufsize);
    bytes = readlink(path, link_to, bufsize);
    return link_to;
}

char *generate_lflg_string(char *path, t_lf_info *info, char* name) {
    
    
    char *result_str = mx_strnew_with(get_lf_table_width(info) + mx_strlen(path), ' ');
    int shift = 0;

    struct stat stat_info;
    
    lstat(path, &stat_info);
    

    // RIGHTS
    char *rights = NULL;
    rights = fmode_to_char(stat_info.st_mode, path);
    mx_strcpy_inf(result_str, rights);
    shift += (info->len_rights + 1); // 1 space

    // LINKS
    char *links = NULL;
    links = mx_itoa(stat_info.st_nlink);
    int len_links = mx_strlen(links);
    
    int spaces = info->len_links - len_links;
    shift += spaces;
    mx_strcpy_inf(result_str + shift, links);
    shift -= spaces;
    shift += (info->len_links + 1);


    // USER AND GROUP
    char *user = NULL; 
    user = get_usr(stat_info.st_uid);

    mx_strcpy_inf(result_str + shift, user);
    shift += (info->len_user + 2);
    
    char *group = NULL;
    group = get_group(stat_info.st_gid);
    mx_strcpy_inf(result_str + shift, group);
    shift += (info->len_group + 2);


    // SIZE
    char *size = NULL;
    size = mx_itoa(stat_info.st_size);
    spaces = info->len_size - mx_strlen(size);
    shift += spaces;
    mx_strcpy_inf(result_str + shift, size);
    shift -= spaces;
    shift += (info->len_size + 1);


    // CHANGE DATE
    char *date = NULL;
    date = get_fchange_date(stat_info.st_mtimespec.tv_sec);
    mx_strcpy_inf(result_str + shift, date);
    shift += (12 + 1);


    mx_strcpy(result_str + shift, name);

    char* link_to = NULL;
    if(S_ISLNK(stat_info.st_mode)) {
        link_to = get_link(path);
        result_str = mx_strjoin_nleak(result_str, " -> ");
        result_str = mx_strjoin_nleak(result_str, link_to);
        free(link_to);
    }


    free(group);
    free(date);
    free(size);
    free(user);
    free(links);
    free(rights);
    return result_str;
}

void set_lf_info_for_path(t_lf_info** info, char* full_path) {
        
    if (*info == NULL) {
        *info = create_lf_info();
    }

    struct stat *stat_info = malloc(sizeof(struct stat));


    if(lstat(full_path, stat_info) != -1) {


        (*info)->total += stat_info->st_blocks;

        (*info)->len_rights = 11;

        // LINKS
        char *links = NULL;
        links = mx_itoa(stat_info->st_nlink);
        
        int len_links = mx_strlen(links);
        if (len_links > (*info)->len_links) (*info)->len_links = len_links;

        // USER AND GROUP
        char *user = NULL; 
        user = get_usr(stat_info->st_uid);        
        if(user) {
            int len_user = mx_strlen(user);
            if (len_user > (*info)->len_user) (*info)->len_user  = len_user;
        }
        //ß
        
        char *group = NULL;
        group = get_group(stat_info->st_gid);
        if (group) {
            int len_group = mx_strlen(group);
            if (len_group > (*info)->len_group) (*info)->len_group = len_group;
        }
        
        // SIZE
        if(!stat_info) mx_printerr("there\n");
        char *size = NULL;
        size = mx_itoa(stat_info->st_size);

        int len_size = mx_strlen(size);
        if (len_size > (*info)->len_size) (*info)->len_size = len_size;

        free(group);
        free(size);
        free(user);
        free(links);
    }
    free(stat_info);
}


void set_lf_info(t_lf_info** info, struct dirent** dirents, int length, char *path) {

    char *path_to_dir = NULL;
    
    for (int dir_index = 0; dir_index < length; dir_index++) {
        path_to_dir = mx_strjoin(path, dirents[dir_index]->d_name);
        set_lf_info_for_path(info, path_to_dir);
        
        free(path_to_dir);
    }
}

int get_lf_table_width(t_lf_info *info) {

    // 1,1,2,2,1,(1,1),1
    int len_date = 12;

    int width = info->len_rights + 1 +
                info->len_links  + 1 +
                info->len_user   + 2 +
                info->len_group  + 2 +
                info->len_size   + 1 +
                len_date         + 1;
    
    return width;
}

void l_flag_print(char *path, t_lf_info* lf_info) {


    char* full_path = NULL;

    if (path[mx_strlen(path) - 1] != '/') full_path = mx_strjoin(path, "/");
    else full_path = mx_strjoin(path, NULL);

    int dir_length = 0;
    DIR* dir = NULL;
    struct dirent** dirents = generate_dirent_array(full_path, &dir_length, &dir);
    mx_quicksort_dirent(dirents, 0, dir_length - 1);


    free(full_path);

    
    for (int index = 0; index < dir_length; index++) {

        char* full_path = NULL;

        if (path[mx_strlen(path) - 1] != '/') full_path = mx_strjoin(path, "/");
        else full_path = mx_strjoin(path, NULL);

        full_path = mx_strjoin_nleak(full_path, dirents[index]->d_name);

        char *lflg_string = generate_lflg_string(full_path, lf_info, dirents[index]->d_name);
        
        free(full_path);
        

        mx_printstr(lflg_string);
        mx_printchar('\n');

        free(lflg_string);

    }
    closedir(dir);
    free_dirents(&dirents, dir_length);
}
