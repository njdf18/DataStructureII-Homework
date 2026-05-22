#include "QuickSort.hpp"

#include <utility>

void quickSort(int arr[], const int left, const int right) {
    if (left < right) {
        int i = left, j = right + 1, pivot = arr[left];
        do {
            do i++; while (arr[i] < pivot);
            do j--; while (arr[j] > pivot);
            if (i <= j) {
                std::swap(arr[i], arr[j]);
            }
        } while (i < j);
        std::swap(arr[left], arr[j]);

        quickSort(arr, left, j - 1);
        quickSort(arr, j + 1, right);
    }
}
