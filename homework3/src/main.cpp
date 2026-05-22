#include <iostream>
#include <ctime>

#include "InsertionSort.hpp"
#include "MergeSort.hpp"

#include "QuickSort.hpp"
#include "HeapSort.hpp"

using namespace std;

//產生 worst-case data
//n, n-1, n-2 ... 1
void generateWorstCase(int arr[], int n)
{
    for (int i = 0; i < n; i++)
    {
        arr[i] = n - i;
    }
}

//複製陣列
void copyArray(int source[], int target[], int n)
{
    for (int i = 0; i < n; i++)
    {
        target[i] = source[i];
    }
}

int main()
{
    //測試資料大小
    int size[] = { 500, 1000, 2000, 3000, 4000, 5000, 10000 };

    for (int n : size)
    {
        //建立測試資料
        int* data = new int[n];

        //給不同排序使用的陣列
        int* insertionArr = new int[n];
        int* mergeArr = new int[n];
        int* quickArr = new int[n];
        int* heapArr = new int[n];

        //產生 worst-case data
        generateWorstCase(data, n);

        //複製資料
        copyArray(data, insertionArr, n);
        copyArray(data, mergeArr, n);

         //Insertion Sort測量
         clock_t start1 = clock();
         insertionSort(insertionArr, n);
         clock_t end1 = clock();
         double insertionTime = double(end1 - start1) / CLOCKS_PER_SEC * 1000;

        //Merge Sort測量
         clock_t start2 = clock();
         mergeSort(mergeArr, 0, n - 1);
         clock_t end2 = clock();
         double mergeTime = double(end2 - start2) / CLOCKS_PER_SEC * 1000;

        //Quick Sort測量
         clock_t start3 = clock();
         quickSort(quickArr, 0, n - 1);
         clock_t end3 = clock();
         double quickTime = double(end3 - start3) / CLOCKS_PER_SEC * 1000;

        clock_t start4 = clock();
        heapSort(heapArr, n);
        clock_t end4 = clock();
        double heapTime = double(end4 - start4) / CLOCKS_PER_SEC * 1000;

        //輸出結果
        cout << "n = " << n << endl;
        cout << "Insertion Sort Time: " << insertionTime << " ms" << endl;
        cout << "Merge Sort Time: " << mergeTime << " ms" << endl;
        cout << "Quick Sort Time: " << quickTime << " ms" << endl;
        cout << "Heap Sort Time: " << heapTime << " ms" << endl;
        cout << "------------------------" << endl;

        //釋放記憶體
        delete[] data;
        delete[] insertionArr;
        delete[] mergeArr;
        delete[] quickArr;
        delete[] heapArr;
    }

    return 0;
}
