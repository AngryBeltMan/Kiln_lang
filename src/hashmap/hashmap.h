#ifndef HASHMAP
#define HASHMAP
#define HASHMAP_SIZE 5
typedef struct Index {
    char* name;
    void* value;
} Index;

typedef struct HashMap {
    Index**list;
} HashMap;

HashMap HASHMAP_new(void);
void HASHMAP_insert(HashMap*hashmap,char* name, void* value);
void* HASHMAP_get(HashMap*hashmap,char* name);
void HASHMAP_drop(HashMap hashmap);
#endif
