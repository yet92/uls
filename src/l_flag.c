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
    result = mx_strjoin_nleak(result, ( (mode & S_IXUSR) ? "x" : "-"));
    result = mx_strjoin_nleak(result, ( (mode & S_IRGRP) ? "r" : "-"));
    result = mx_strjoin_nleak(result, ( (mode & S_IWGRP) ? "w" : "-"));
    result = mx_strjoin_nleak(result, ( (mode & S_IXGRP) ? "x" : "-"));
    result = mx_strjoin_nleak(result, ( (mode & S_IROTH) ? "r" : "-"));
    result = mx_strjoin_nleak(result, ( (mode & S_IWOTH) ? "w" : "-"));
    result = mx_strjoin_nleak(result, ( (mode & S_IXOTH) ? "x" : "-"));
    
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
    if(pws->pw_name != NULL)
        result_str = mx_strdup(pws->pw_name);

    return result_str;
}

char *get_group(unsigned int gid) {

    // TODO: if grp == NULL add gid as string
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

char *generate_lflg_string(char *path, t_lf_info *info, char* name) {
    
    
    char *result_str = mx_strnew_with(get_lf_table_width(info) + mx_strlen(path), ' ');
    // char *result = result_str;
    int shift = 0;

    struct stat stat_info;
    
    lstat(path, &stat_info);

    // RIGHTS
    char *rights = NULL;
    rights = fmode_to_char(stat_info.st_mode, path);
    // result_str = mx_strjoin(result_str, rights);
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

    // result_str = mx_strjoin(result_str, links);

    // USER AND GROUP
    char *user = NULL; 
    user = get_usr(stat_info.st_uid);

    mx_strcpy_inf(result_str + shift, user);
    shift += (info->len_user + 2);
    //
    // result_str = mx_strjoin(result_str, user);
    
    char *group = NULL;
    group = get_group(stat_info.st_gid);
    mx_strcpy_inf(result_str + shift, group);
    shift += (info->len_group + 2);

    // result_str = mx_strjoin(result_str, group);

    // SIZE
    char *size = NULL;
    size = mx_itoa(stat_info.st_size);
    
    spaces = info->len_size - mx_strlen(size);
    shift += spaces;
    mx_strcpy_inf(result_str + shift, size);
    shift -= spaces;
    shift += (info->len_size + 1);

    // result_str = mx_strjoin(result_str, size);

    // CHANGE DATE
    char *date = NULL;
    date = get_fchange_date(stat_info.st_mtimespec.tv_sec);
    mx_strcpy_inf(result_str + shift, date);
    shift += (12 + 1);

    // printf("\n\n DATE: %s\n\n", date);
    if(S_ISLNK(stat_info.st_mode)) {
        ssize_t bytes, bufsize;
        bufsize = stat_info.st_mode + 1;

        if (stat_info.st_mode == 0){
            bufsize = PATH_MAX;
        }

        char *buff = mx_strnew(bufsize);
        bytes = readlink(path, buff, bufsize);
        if (bytes != -1){
            name = mx_strjoin(name, " -> ");
            name = mx_strjoin_nleak(name, buff);
        }
        mx_strdel(&buff);
        mx_strcpy(result_str + shift, name);

    } else 
        mx_strcpy(result_str + shift, name);

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

        struct stat stat_info;

        stat(full_path, &stat_info);

        (*info)->total += stat_info.st_blocks;

        (*info)->len_rights = 11;

        // LINKS
        char *links = NULL;
        links = mx_itoa(stat_info.st_nlink);
        
        int len_links = mx_strlen(links);
        if (len_links > (*info)->len_links) (*info)->len_links = len_links;
        // 

        // USER AND GROUP
        char *user = NULL; 
        user = get_usr(stat_info.st_uid);
        

        int len_user = mx_strlen(user);
        if (len_user > (*info)->len_user) (*info)->len_user  = len_user;

        //ß
        
        char *group = NULL;
        group = get_group(stat_info.st_gid);
        if (group) {
            int len_group = mx_strlen(group);
            if (len_group > (*info)->len_group) (*info)->len_group = len_group;
        }
        

        // SIZE
        char *size = NULL;
        size = mx_itoa(stat_info.st_size);

        int len_size = mx_strlen(size);
        if (len_size > (*info)->len_size) (*info)->len_size = len_size;


        free(group);
        free(size);
        free(user);
        free(links);
}

void set_lf_info(t_lf_info** info, struct dirent** dirents, int length, char *path) {

    char *path_to_dir = NULL;
    
    for (int dir_index = 0; dir_index < length; dir_index++) {
        path_to_dir = mx_strjoin(path, dirents[dir_index]->d_name);
        set_lf_info_for_path(info, path_to_dir);
        free(path_to_dir);
        // free(rights);
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

    // t_lf_info* lf_info  = NULL;

    char* full_path = NULL;

    if (path[mx_strlen(path) - 1] != '/') full_path = mx_strjoin(path, "/");
    else full_path = mx_strjoin(path, NULL);

    int dir_length = 0;
    DIR* dir = NULL;
    struct dirent** dirents = generate_dirent_array(full_path, &dir_length, &dir);
    mx_quicksort_dirent(dirents, 0, dir_length - 1);

    // set_lf_info(&lf_info, dirents, dir_length, full_path);

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
    free(lf_info);
    free(dirents);
}
