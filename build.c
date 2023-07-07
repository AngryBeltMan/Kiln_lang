#include<stdlib.h>
#include<string.h>
typedef struct __HeapArray {
 void** _vars; uint _max; uint _size;
 } __HeapArray;
 __HeapArray __HeapArrayNew() { 
 __HeapArray heap_array;
 heap_array._vars = malloc(sizeof(void**));
 heap_array._max = sizeof(void**);
 heap_array._size = 0;
 return heap_array;
 } 
void __HeapArrayAppend( __HeapArray* heap_array, void* value_ref) {
 if (heap_array->_size + 8 > heap_array->_max) {
 heap_array->_max *= 2;
 heap_array->_vars = realloc(heap_array->_vars, heap_array->_max);
 }
 heap_array->_vars[(heap_array->_size/8)] = value_ref;
 heap_array->_size += 8;
}
 void __HeapArrayDrop(__HeapArray heap_array) {
 for (int i = 0; i < (heap_array._size/sizeof(void**)); ++i) {
 free(heap_array._vars[i]); 
heap_array._vars[i] = NULL; 
} free(heap_array._vars);
 } 
int main(void) {
__HeapArray ___heap = __HeapArrayNew();
char* hello = malloc(sizeof(char) * 129);
__HeapArrayAppend(&___heap, hello);
strcpy(hello,"hello world");
char* cool = "9999";
__HeapArrayDrop(___heap);
return 0; 
 }