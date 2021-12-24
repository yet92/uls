#include "flags.h"

char *fmode_to_char(int mode) {
    char *result = mx_strnew(0);

    result = mx_strjoin(result, ( (S_ISDIR(mode)) ? "d" : "-"));
    result = mx_strjoin(result, ( (mode & S_IRUSR) ? "r" : "-"));
    result = mx_strjoin(result, ( (mode & S_IWUSR) ? "w" : "-"));
    result = mx_strjoin(result, ( (mode & S_IXUSR) ? "x" : "-"));
    result = mx_strjoin(result, ( (mode & S_IRGRP) ? "r" : "-"));
    result = mx_strjoin(result, ( (mode & S_IWGRP) ? "w" : "-"));
    result = mx_strjoin(result, ( (mode & S_IXGRP) ? "x" : "-"));
    result = mx_strjoin(result, ( (mode & S_IROTH) ? "r" : "-"));
    result = mx_strjoin(result, ( (mode & S_IWOTH) ? "w" : "-"));
    result = mx_strjoin(result, ( (mode & S_IXOTH) ? "x" : "-"));

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

    char *result_str = NULL;

    struct group *grp;

    grp = getgrgid(gid);
    if(grp->gr_name != NULL) {
        result_str = mx_strjoin(result_str, "  ");
        result_str = mx_strjoin(result_str, grp->gr_name);
    }

    return result_str;
}

char *get_fchange_date(const time_t date) {
    char *clock;

    clock=  ctime(&(date));
    return clock;
}

char *generate_lflg_string(char *name) {
    char *result_str = mx_strnew(0);
    
    struct stat stat_info;
    
    stat(name, &stat_info);

    // RIGHTS
    char *rights = NULL;
    rights = fmode_to_char(stat_info.st_mode);
    result_str = mx_strjoin(result_str, rights);

    // LINKS
    char *links = NULL;
    links = mx_itoa(stat_info.st_nlink);

    // 

    result_str = mx_strjoin(result_str, links);

    // USER AND GROUP
    char *user = NULL; 
    user = get_usr(stat_info.st_uid);
    
    //
    result_str = mx_strjoin(result_str, user);
    
    char *group = NULL;
    group = get_group(stat_info.st_gid);

    result_str = mx_strjoin(result_str, group);

    // SIZE
    char *size = NULL;
    size = mx_itoa(stat_info.st_size);
    result_str = mx_strjoin(result_str, size);

    // CHANGE DATE
    char *date = NULL;
    date = get_fchange_date(stat_info.st_mtimespec.tv_sec);

    printf("\n\n DATE: %s\n\n", date);

    free(size);
    free(user);
    free(links);
    free(rights);
    return result_str;
}
