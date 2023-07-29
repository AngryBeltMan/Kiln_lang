#include<stdlib.h>
typedef struct Number {  int numberator; int denominator ; } Number;
int __METHOD_Numberadd_one(Number number, int num) {
number.denominator = num;
return number.numberator;
}
int __MAIN() {
Number wow = 10;
__METHOD_Numberadd_one(wow,10);
}

int main() {
__MAIN( );
return 0;
 }