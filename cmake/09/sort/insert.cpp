#include <stdio.h>
#include "sort.h"
#include "calc.h"

void insertion_sort(int arr[], int n) {

    // 这里使用了add方法，则sort的静态库就要依赖于calc的静态库
    int num = add(2, 3);
    printf("test number = %d \n", num);

    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        // 将大于 key 的元素向后移动一位
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}