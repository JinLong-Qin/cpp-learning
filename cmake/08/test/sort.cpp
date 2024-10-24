#include <stdio.h>
#include "sort.h"

// 打印数组函数
void print_array(int arr[], int n) {
    for (int i = 0; i < n; ++i) {
        printf("%d \t", arr[i]);
    }
    printf("\n");
}

int main(){
    int original_arr1[] = {64, 25, 12, 22, 11};
    int original_arr2[] = {64, 25, 12, 22, 11};
    int n = 5;

    print_array(original_arr1, n);
    insertion_sort(original_arr1, n);
    print_array(original_arr1, n);

    print_array(original_arr2, n);
    selection_sort(original_arr2, n);
    print_array(original_arr2, n);
    
}