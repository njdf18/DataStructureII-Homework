#include "InsertionSort.hpp"

void insertionSort(int arr[], int n)
{
    //從第二個元素開始排序
    for (int i = 1; i < n; i++)
    {
        int key = arr[i];
        int j = i - 1;

        //將比key大的元素往後移
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }

        //放入正確位置
        arr[j + 1] = key;
    }
}
