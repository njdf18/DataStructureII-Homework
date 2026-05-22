#include "MergeSort.hpp"

//合併兩個已排序區間
void merge(int arr[], int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    //建立暫存陣列
    int* L = new int[n1];
    int* R = new int[n2];

    //複製左半部資料
    for (int i = 0; i < n1; i++)
    {
        L[i] = arr[left + i];
    }

    //複製右半部資料
    for (int j = 0; j < n2; j++)
    {
        R[j] = arr[mid + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = left;

    //合併回原本陣列
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }

        k++;
    }

    //左半剩餘元素
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    //右半剩餘元素
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }

    //釋放記憶體
    delete[] L;
    delete[] R;
}

void mergeSort(int arr[], int left, int right)
{
    if (left < right)
    {
        int mid = (left + right) / 2;

        //排序左半部
        mergeSort(arr, left, mid);

        //排序右半部
        mergeSort(arr, mid + 1, right);

        //合併
        merge(arr, left, mid, right);
    }
}