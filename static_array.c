#include <stdlib.h>
#include <stdio.h>


int main(int argc, char **argv)
{
    int len;
    printf("Enter len: ");
    scanf("%d", &len);
    // int len = atoi(argv[1]);

    printf("%d\n", len);


    int array [len]; // Not known at compile time!? Works up to just around len = 2*10^6
    // int *array = (int *)malloc(len *sizeof(int)); // Known at compile time: works for high len

    // printf("%lu\n", sizeof(array));
    for (size_t i = 0; i < len ; i++)
    {
        array[i] = i;
    }
}