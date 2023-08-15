#include "contents.c"
#include <stdio.h>
int main() {
    Contents con = CONTENTS_from("./hello.kiln");
    CONTENTS_pop_back(&con, 4);
    CONTENTS_append(&con,'c');
    printf("%s\n", con.file);
    CONTENTS_drop(con);
}
