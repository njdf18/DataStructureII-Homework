# 41143203
41143124

作業三

## 解題說明

本題要求實現四種內部排序法(Internal Sorting)，並計算每個排序法分別在n=500到10000時的執行時間。
1. 插入排序法(Insertion Sort)
2. 快速排序法(Quick Sort)
3. 合併排序法(Merge Sort)
4. 堆積排序法(Heap Sort)

### 解題策略

1. 插入排序法(Insertion Sort)
   - asdfjkl
2. 快速排序法(Quick Sort)
   - asdfjkl
3. 合併排序法(Merge Sort)
   - 使用遞迴方式完成
4. 堆積排序法(Heap Sort)
   - asdfjkl

## 程式實作

以下為主要程式碼：

### Insertion Sort
```cpp
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
```

### Quick Sort
```cpp
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
```

### Merge Sort
```cpp
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
```

### Heap Sort
```cpp
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
```

## 效能分析

本作業實作的四種排序演算法之效能分析如下：

| 排序法 | 平均時間複雜度 | 最差時間複雜度 | 空間複雜度 |
| :--- | :---: | :---: | :---: |
| **Insertion Sort** | $O(n^2)$ | $O(n^2)$ | $O(1)$ |
| **Merge Sort** | $O(n \log n)$ | $O(n \log n)$ | $O(n)$ |
| **Quick Sort** | $O(n \log n)$ | $O(n^2)$ | $O(\log n)$ |
| **Heap Sort** | $O(n \log n)$ | $O(n \log n)$ | $O(1)$ |

1. **時間複雜度**：
   - Insertion Sort 在資料量大時效率較低。
   - Merge, Quick, Heap Sort 均能在 $O(n \log n)$ 時間內完成，適合處理大規模數據。
2. **空間複雜度**：
   - Merge Sort 需要額外的陣列空間來合併子問題，因此空間複雜度最高。
   - Insertion Sort 和 Heap Sort 為原地(In-place)排序，空間效率最佳。
   - Quick Sort 的空間消耗主要來自遞迴呼叫產生的堆疊空間。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $n$ | Insertion Sort | Merge Sort | Quick Sort | Heap Sort |
|:---:|:---:|:---:|:---:|:---:|:---:|
| 測試一 | 500 | 0.203 ms | 0.036 ms | 0.014 ms | 0.007 ms |
| 測試二 | 1000 | 0.82 ms | 0.071 ms | 0.031 ms | 0.011 ms |
| 測試三 | 2000 | 3.216 ms | 0.135 ms | 0.069 ms | 0.031 ms |
| 測試四 | 3000 | 6.861 ms | 0.214 ms | 0.422 ms | 0.037 ms |
| 測試五 | 4000 | 14.092 ms | 0.28 ms | 0.887 ms | 0.078 ms |
| 測試六 | 5000 | 19.669 ms | 0.358 ms | 0.268 ms | 0.071 ms |
| 測試七 | 10000 | 80.021 ms | 0.743 ms | 0.438 ms | 0.133 ms |

### 編譯與執行指令

```bash
g++ -std=c++17 main.cpp InsertionSort.cpp QuickSort.cpp MergeSort.cpp HeapSort.cpp -o main
./main
```

### 結論

1. 成功實作四種內部排序法：Insertion Sort, Merge Sort, Quick Sort, 及 Heap Sort。
2. 透過測試不同大小的資料集 ($n=500$ 到 $10000$)，驗證了各排序法的效能表現。
3. 實驗結果顯示，當資料量增加時，Insertion Sort 的執行時間增加最快（$O(n^2)$），而其他三種 $O(n \log n)$ 的排序法表現相對穩定且快速。

## 申論及開發報告

### 排序法效能比較分析

在本作業中，我們比較了四種不同的內部排序演算法。根據測試結果，可以觀察到以下現象：

1. **Insertion Sort 的效能瓶頸**
   Insertion Sort 的時間複雜度為 $O(n^2)$。從測試數據中可以看到，當 $n$ 從 500 增加到 10000 時，其執行時間從 0.203 ms 劇增至 80.021 ms，呈現平方倍數的成長。這驗證了它不適合處理大量資料的特性。

2. **$O(n \log n)$ 演算法的優勢**
   Merge Sort、Quick Sort 和 Heap Sort 的時間複雜度均為 $O(n \log n)$。在 $n=10000$ 時，它們的執行時間均遠低於 1 ms（Merge: 0.743 ms, Quick: 0.438 ms, Heap: 0.133 ms），由此可看出其效率。

3. **實作細節與觀察**
   - **Quick Sort** 在某些情況下（如 $n=3000, 4000$）出現了效能波動（0.422 ms, 0.887 ms），這可能是由於 Pivot 選擇或資料初始分佈影響了分割的平衡性。
   - **Heap Sort** 在本次測試中表現最為穩定且快速，即使在 $n=10000$ 時也僅需 0.133 ms。
   - **Merge Sort** 雖然穩定，但由於需要額外的記憶體空間進行合併操作，在執行時間上略比 Heap Sort 差。
