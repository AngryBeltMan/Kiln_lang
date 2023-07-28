#include "contents.c"
#include <stdio.h>
int main() {
    Contents con = CONTENTS_new();
    CONTENTS_append_str(&con, "self value");
    Contents replace_str = CONTENTS_replace(&con, "self", "int");
    printf("%s\n",replace_str.file);
    CONTENTS_drop(con);
    CONTENTS_drop(replace_str);
}
