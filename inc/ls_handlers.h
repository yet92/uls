#ifndef LS_HANDLERS
#define LS_HANDLERS

#include "basic.h"
#include "l_flag.h"
#include "ls_errors.h"
#include "multiply_columns.h"
#include "one_column.h"

typedef struct s_flags {
    bool no_flag;
    bool l_flag;
    bool wrong_flag;
}           t_flags;

#define IS_FLAG(data) ((data[0] == '-') ? true : false)

void check_flag(char *data, t_flags *flag, char* illegal_flag_option);

void check_args(int argc, char **argv);

void files_handler(char** pathes, int pathes_number, t_flags* flags, t_lf_info_node** lf_info);

void directories_handler(char** pathes, int pathes_number, t_flags* flags, t_lf_info_node** lf_info);

void correct_args_handler(char** pathes, int pathes_number, t_flags* flags);

char *generate_full_path(char *path);

#endif /*LS_HANDLERS*/
