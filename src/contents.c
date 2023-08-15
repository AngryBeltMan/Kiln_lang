#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#ifndef CONTENTS
#define CONTENTS
#define CONTENTS_append(con,c) /*printf("appended on line %i on file %s\n",__LINE__,__FILE__);*/ CONTENTSappend(con,c);
typedef struct Contents {
    char* file;
    unsigned int size;
    unsigned int max;
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

// get a slice of the CONTENT given a start and end
static inline Contents CONTENTS_slice_range(Contents *P_con,unsigned int start,unsigned int end) {
    Contents cont = CONTENTS_new();
    assert(cont.size > end && "ERROR: End length is greater than content");
    assert(start < end && "ERROR: start is greater than end");
    for (;start <= end; ++start) {
        CONTENTS_append(&cont, P_con->file[start]);
    }
    return cont;
}
static inline Contents CONTENTS_from(char* string) {
    Contents con = CONTENTS_new();
    CONTENTS_append_str(&con, string);
    return con;
}
Contents CONTENTS_from_char_slice_range(char *P_con,unsigned int start,unsigned int end) {
    Contents cont = CONTENTS_new();
    for (;start <= end; ++start) {
        CONTENTS_append(&cont, P_con[start]);
    }
    return cont;
}
// Removes a set amount of characters from the back. Should be faster than using CONTENTS_slice_range to remove characters.
static inline void CONTENTS_pop_back(Contents *P_con, unsigned int amount) {
    assert(P_con->size > amount && "ERROR: Contents is not large enough");
    const unsigned int size = P_con->size - amount;
    char* new_file = malloc(size + 8);
    assert(new_file && "ERROR: Contents failed to malloc");
    memmove(new_file,P_con->file , size);
    free(P_con->file);
    P_con->file = new_file;
    P_con->size = size;
    P_con->max = size + 8;
}

// DOCS: append to contents using a formatted string.
// assert(!strcmp(content.file, "hello "));
// assert(!strcmp(world,"world"));
// CONTENTS_append_formatted(&content, "%s!", world);
// assert(!strcmp(content.file, "hello world!"));
static inline void CONTENTS_append_formatted(Contents*P_con,const char *fmt, ...)
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

static inline Contents CONTENTS_from_formatted(const char *fmt, ...) {
    Contents con = CONTENTS_new();
    CONTENTS_append_formatted(&con, fmt);
    return con;
}
static inline void CONTENTS_pop_front(Contents *P_con, const unsigned int amount) {
    const int length = P_con->size;
    if (amount > length) {
        fprintf(stderr, "Pop amount is greater than str length\n");
        return;
    }
    char* new_str = malloc((length - amount) + 1);
    assert(new_str != NULL && "Failed to malloc");
    new_str[(length - amount)] = '\0';
    for (int i = amount;i < length;++i) {
        new_str[i - amount] = P_con->file[i];
    }
    free(P_con->file);
    P_con->file = new_str;
    P_con->size = length - amount;
    P_con->max = (length - amount) + 1;
}

// modifies the Contents argument to be only the first part of the Content and returns the other half of the Content.
// Example:
// assert(cool.file == "cool");
// char*  other_cool = CONETENTS_split_at(&cool, 1);
// assert(cool.file == "co");
// assert(other_cool.file == "ol");
char* CONTENTS_split_at(Contents *P_con, unsigned int index) {
    assert(P_con->size >= index);
    if (!index) {
        char* first_segment = malloc(1);
        strcpy(first_segment, "");
        return first_segment;
    }
    char* first_segment = malloc(index);
    memmove(first_segment, P_con->file, index);
    CONTENTS_pop_front(P_con, index);
    return first_segment;
}
Contents CONTENTS_replace(Contents *P_con, char* from, char* to) {
    const unsigned int from_length = strlen(from);
    unsigned int matches = 0;
    unsigned int index = 0;
    do {
        if (P_con->file[index] == from[matches]) { ++matches; } else { matches = 0;}
        ++index;
    } while(!(index < P_con->size) | (matches != from_length));
    if (matches == from_length) {
        char* first = CONTENTS_split_at(P_con, index - from_length);
        CONTENTS_pop_front(P_con, from_length);
        Contents replaced_string = CONTENTS_new();
        CONTENTS_append_formatted(&replaced_string, "%s%s%s",first, to, P_con->file);
        free(first);
        return replaced_string;
    }
    return CONTENTS_new();
}

static inline void CONTENTS_drop(Contents con) {
    free(con.file);
    con.file = NULL;
}
#endif
