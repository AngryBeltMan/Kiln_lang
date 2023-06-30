#include<stdio.h>
#include<stdlib.h>
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
void __HeapArrayAppend( __HeapArray* heap_array, void** value_ref) {
 if (heap_array->_size + sizeof(void**) > heap_array->_max) {
 heap_array->_max *= 2;
 heap_array->_vars = realloc(heap_array->_vars, heap_array->_max);
 }
 heap_array->_vars[(heap_array->_size/sizeof(void**))] = value_ref;
 }
 void __HeapArrayDrop(__HeapArray heap_array) {
 for (int i = 0; i < (heap_array._size/sizeof(void**)); ++i) {
 free(heap_array._vars[i]); 
heap_array._vars[i] = NULL; 
} } 
int main(void) {
int*hello=malloc(sizeof(int*));
__HeapArray ___heap = __HeapArrayNew();
__HeapArrayAppend(&___heap,(void*)hello);
*hello=10;
__HeapArrayDrop(___heap);
return 0; 
 }