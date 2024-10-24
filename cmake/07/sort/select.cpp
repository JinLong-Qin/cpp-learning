#include <stdio.h>
#include "sort.h"

void selection_sort(int arr[], int n) {
    int i, j, min_idx, temp;

    // 依次选择最小的元素
    for (i = 0; i < n - 1; i++) {
        // 假设当前索引 i 的元素是最小的
        min_idx = i;

        // 在未排序部分寻找最小的元素
        for (j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }

        // 交换当前索引 i 的元素和找到的最小元素
        if (min_idx != i) {
            temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
        }
    }
}