#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "contents.c"

int main() {
    Contents con = CONTENTS_new();
    CONTENTS_append_str(&con,"hello");
    Contents hel = CONTENTS_slice_range(&con,0,2);
    printf("%s\n",hel.file);
    free(con.file);
    free(hel.file);
    return 0;
}
