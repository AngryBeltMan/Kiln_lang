#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#ifndef CONTENTS
#define CONTENTS
#define CONTENTS_append(con,c) /*printf("appended on line %i on file %s\n",__LINE__,__FILE__);*/ CONTENTSappend(con,c);
typedef struct Contents {
    char* file;
    uint size;
    uint max;
} Contents;

Contents CONTENTS_new() {
    Contents con;
    con.size = 0;
    con.max = 16;
    con.file = malloc(16);
    if (!con.file) { printf("ERROR: failed to malloc content %s\n",__FUNCTION__); }
    con.file[con.size]= '\0';
    return con;
}

static inline void CONTENTSappend(Contents *con,char c) {
    if (con->size + 2 > con->max) {
        con->max *= 2;
        con->file = realloc(con->file,con->max);
        if (!con->file) { printf("ERROR: failed to realloc %s\n",__FUNCTION__); }
    }
    con->file[con->size] = c;
    con->file[con->size + 1]= '\0';
    con->size += 1;
}

static inline void CONTENTS_append_str(Contents *con,char* string) {
    while (con->size + strlen(string) > con->max) {
        con->max *= 2;
        con->file = realloc(con->file,con->max);
        if (!con->file) { printf("ERROR: failed to realloc %s\n",__FUNCTION__); }
    }
    strcat(con->file, string);
    con->size += strlen(string);
}

static inline Contents CONTENTS_slice_range(Contents *P_con,uint start,uint end) {
    Contents cont = CONTENTS_new();
    for (;start <= end; ++start) {
        CONTENTS_append(&cont, P_con->file[start]);
    }
    return cont;
}

Contents CONTENTS_from_char_slice_range(char *P_con,uint start,uint end) {
    Contents cont = CONTENTS_new();
    for (;start <= end; ++start) {
        CONTENTS_append(&cont, P_con[start]);
    }
    return cont;
}

static inline void CONTENTS_append_formated(Contents*P_con,const char *fmt, ...)
{
    va_list args;
    size_t  len;
    char   *space;

    va_start(args, fmt);
    len = vsnprintf(0, 0, fmt, args);
    va_end(args);
    if ((space = malloc(len + 1)) != 0)
    {
         va_start(args, fmt);
         vsnprintf(space, len+1, fmt, args);
         va_end(args);
         CONTENTS_append_str(P_con,space);
         free(space);
    } else {
        printf("space was not malloced\n");
    }
}
#endif
