#include <stdlib.h>
#include <stdio.h>


void swap();
void sort();

int main(){


  int arr[5] = {2,3,1,0,5};
  int perm[5] = {0,1,2,3,4};
  sort(arr, perm, 0, 5);


  for (int i=0; i<5; i++){
    printf("%d\n", arr[perm[i]]);
  }
  printf("%s\n", "\n" );

  for (int i=0; i<5; i++){
    printf("%d\n", perm[i]);
  }
  return 0;
}


void swap(int *a, int *b)
    {
      int t=*a; *a=*b; *b=t;
    }

void sort(int arr[], int perm[], int beg, int end)
  {
    if (end > beg + 1) {
      int piv = arr[perm[beg]], l = beg + 1, r = end;
      while (l < r) {
        if (arr[perm[l]] <= piv)
          l++;
        else
          swap(&perm[l], &perm[--r]);
      }
      swap(&perm[--l], &perm[beg]);
      sort(arr, perm, beg, l);
      sort(arr, perm, r, end);
    }
}
