#include <stdio.h>
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
    con.max = 8;
    con.file = malloc(8);
    con.file[con.size]= '\0';
    return con;
}

void CONTENTSappend(Contents *con,char c) {
    if (con->size + 2 > con->max) {
        con->max *= 2;
        con->file = realloc(con->file,con->max);
    }
    con->file[con->size] = c;
    con->file[con->size + 1]= '\0';
    con->size += 1;
}
void CONTENTS_append_str(Contents *con,char* string) {
    while (con->size + strlen(string) > con->max) {
        con->max *= 2;
        con->file = realloc(con->file,con->max);
    }
    strcat(con->file, string);
    con->size += strlen(string);
}
#endif
