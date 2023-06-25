#include <stdio.h>
#include <stdlib.h>

#ifndef CONTENTS
#define CONTENTS
typedef struct Contents {
    char* file;
    uint size;
    uint max;
} Contents;

Contents CONTENTS_new() {
    Contents con;
    con.file = malloc(8);
    con.size = 0;
    con.max = 8;
    return con;
}

void CONTENTS_append(Contents *con,char c) {
    if (con->size + 1 > con->max) {
        con->max *= 2;
        con->file = realloc(con->file,con->max);
    }
    con->file[con->size] = c;
    con->size += 1;
}
#endif
