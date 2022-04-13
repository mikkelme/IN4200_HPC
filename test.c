// #include <stdio.h>
// #include <float.h>



#include <stdio.h>
#include <stdlib.h>
int main() {
    int n, i, *ptr, sum = 0;
    printf("Enter number of elements: ");
    scanf("%d", &n);
    ptr = (int*) malloc(n * sizeof(int));
    if(ptr == NULL) {
        printf("Error! memory not allocated."); exit(-1);
    }
    printf("Enter elements: ");
    for (i = 0; i < n; ++i) {
        scanf("%d", &(ptr[i]));
        sum += ptr[i];
    }
    printf("Sum = %d\n", sum);
    free(ptr);
    return 0;
}

// void func_1();
// int a = 10;
//
// int main(){
//   // a = 10;
//   // printf("a = %d\n", a );
//   //
//   // func_1();
//   //
//   // printf("a = %d\n", a );
//   //
//
//
//   // printf("Hello World! \n");
//   // printf("Storage size for int: %ld \n", sizeof(int));
//
//   // Array
//   char var2[10];
//
//   printf("Adress of array: %p \n", &var2);
//
//
//
//   //
//   // printf("Storage size for float: %lu \n", sizeof(float));
//   // printf("Minimum float positive value: %E\n", FLT_MIN);
//   // printf("Maximum float positive value: %E\n", FLT_MAX);
//
//   return 0;
// }
//
//
// void func_1(){
//   a += 1;
//   a++;
//   // printf("a =  %d \n", a);
//
// }
//
//
//  // compile: make test, gcc test
