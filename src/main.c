#include "libmx.h"
#include "basic.h"
#include "flags.h"
#include "multiply_columns.h"

int main(int argc, char* argv[]) {
    
    argc = 0;
    argv = NULL;
    // multiply_columns_print("./libmx");
    char *str = generate_lflg_string("/Users/yzhuravlov/Applications");
    mx_printstr(str);

    return 0;
}

