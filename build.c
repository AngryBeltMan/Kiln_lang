#include<stdlib.h>
typedef struct Number {  int numberator; int denominator ; } Number;
void __METHOD_Numberset(Number* number, int n, int d) {
number.numberator = n;
number.denominator = d;
}
int __MAIN() {
Number wow = 10;
__METHOD_Numberadd_one(wow,10, 12);
}

int main() {
__MAIN( );
return 0;
 }