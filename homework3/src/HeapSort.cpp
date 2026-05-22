#include "HeapSort.hpp"

#include <utility>

void adjust(int arr[], const int root, const int n) {
    int e = arr[root];
    int j = 2 * root;
    for (; j <= n; j *= 2) {
        if (j < n && arr[j] < arr[j + 1])
            j++;
        if (e >= arr[j])
            break;
        arr[j/2] = arr[j];
    }
    arr[j/2] = e;
}

void heapSort(int arr[], int n) {
    for (int i = n / 2; i >= 1; i--)
        adjust(arr, i, n);

    for (int i = n - 1; i >= 1; i--) {
        std::swap(arr[i], arr[i+1]);
        adjust(arr, 1, i);
    }
}