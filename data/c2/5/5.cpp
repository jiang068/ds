/*实验5 排序算法的计算复杂度
实验目的
1. 掌握各种排序算法。
2. 学习测量程序运行时间的方法。
问题描述
在课程学习中，我们已经知道不同的排序算法具有不同的时间复杂度，那么在具体应用中，各种排序算法的运行时间究竟相差多少？通过这个实验，对程序运行时间进行实际的测量，可以直观感受到时间复杂度与问题规模的关系。
实验内容
本实验要求编程实现至少5种排序算法（快速、堆、归并必做，其他选做），并在不同N值（如10000、100000、1000000）的条件下多次运行程序计算平均运行时间。
(1.快速排序（Quick Sort）：一种基于分治法的排序算法，通过选择一个基准元素，将数组分成两部分，一部分小于基准元素，另一部分大于基准元素，然后递归地对这两部分进行排序。
2.堆排序（Heap Sort）：一种基于堆数据结构的排序算法，利用堆的性质来选择最大或最小元素，并将其放在数组的末尾，重复此过程直到数组有序。
3.归并排序（Merge Sort）：一种基于分治法的排序算法，将数组分成两部分，分别排序后再合并，合并时保证合并后的数组有序。)
实现提示
为了公平起见，我们应该使用同一个无序序列作为输入，来测量不同排序算法的运行时间。那么无序序列如何得到？一种方法是，先生成一个长度为N的有序序列，再将该序列随机重排(random shuffle)，从而得到一个长度为N的无序序列。
测量程序的运行时间，我们可以使用C/C++语言提供的计时器。需要注意的是，该计时器的灵敏度比较低，在Windows系统中，一般只有当两组运行时间相差0.1秒以上时，才能认为这两组时间是有差别的。
部分程序代码示例如下：*/

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cstdio>
#include <iostream>

using namespace std; // c++常规写法

typedef int ElemType;

typedef struct {
    ElemType *r;
    int len;
} SqTable;//顺序表

void InitList(SqTable &L, int len) {
    // 0号单元不用
    L.r = (ElemType*)malloc((len+1)*sizeof(ElemType));
    L.len = len;
}//初始化顺序表

void CopyList(SqTable L, SqTable &newL) {
    newL.r = (ElemType*)malloc((L.len+1)*sizeof(ElemType));
    newL.len = L.len;
    memcpy(newL.r, L.r, (L.len+1)*sizeof(ElemType));
}//复制顺序表

// 求一个整数的p次方
int intpow(int n, unsigned int p) {
    int res = 1;
    for (unsigned int i=0; i<p; ++i)
        res *= n;
    return res;
}

// 生成一个随机整数，其取值范围是[0, bound]
int randb(int bound) {
    int r = 0;
    unsigned int power = 0;
    do {
        r *= RAND_MAX;
        r += rand(); ++power;
    } while (intpow(RAND_MAX, power) < bound);
    return r % (bound+1);
}

// 随机打乱一个数组
void RandomShuffleList(SqTable L) {
    ElemType* array = L.r + 1; int n = L.len;
    for (int i=n-1; i>0; --i) {
        int j = randb(i); // 0<=j<=i
        ElemType tmp = array[i]; array[i] = array[j]; array[j] = tmp;
    }
}

//1.快速排序算法
void QuickSort(SqTable &L, int low, int high) {
    if (low < high) {
        int i = low, j = high;
        ElemType pivot = L.r[low];
        while (i < j) {
            while (i < j && L.r[j] >= pivot) --j;
            if (i < j) L.r[i++] = L.r[j];
            while (i < j && L.r[i] <= pivot) ++i;
            if (i < j) L.r[j--] = L.r[i];
        }
        L.r[i] = pivot;
        QuickSort(L, low, i - 1);
        QuickSort(L, i + 1, high);
    }
}
void Quick(SqTable &L) {
    QuickSort(L, 1, L.len);
}

//2.堆排序算法
void HeapAdjust(SqTable &L, int s, int m) {
    ElemType tmp = L.r[s];
    for (int j = 2 * s; j <= m; j *= 2) {
        if (j < m && L.r[j] < L.r[j + 1]) ++j;
        if (tmp >= L.r[j]) break;
        L.r[s] = L.r[j];
        s = j;
    }
    L.r[s] = tmp;
}
void Heap(SqTable &L) {
    for (int i = L.len / 2; i > 0; --i) {
        HeapAdjust(L, i, L.len);
    }
    for (int i = L.len; i > 1; --i) {
        ElemType tmp = L.r[1]; L.r[1] = L.r[i]; L.r[i] = tmp;
        HeapAdjust(L, 1, i - 1);
    }
}

//3.归并排序算法
void Merge(SqTable &L, int low, int mid, int high) {
    ElemType* tmp = (ElemType*)malloc((high-low+1)*sizeof(ElemType));
    int i = low, j = mid+1, k = 0;
    while (i <= mid && j <= high) {
        if (L.r[i] <= L.r[j]) tmp[k++] = L.r[i++];
        else tmp[k++] = L.r[j++];
    }
    while (i <= mid) tmp[k++] = L.r[i++];
    while (j <= high) tmp[k++] = L.r[j++];
    for (k=0; k<high-low+1; ++k) L.r[low+k] = tmp[k];
    free(tmp);
}
void MergeSort(SqTable &L, int low, int high) {
    if (low < high) {
        int mid = (low + high) / 2;
        MergeSort(L, low, mid);
        MergeSort(L, mid + 1, high);
        Merge(L, low, mid, high);
    }
}

//4.冒泡排序
void Bubble(SqTable &L){
    bool change = true;
    for (int i = 1; i < L.len && change; ++i){
        change = false;
        for (int j = 1; j <= L.len - i; ++j){
            if (L.r[j] > L.r[j+1]){
                ElemType tmp = L.r[j];
                L.r[j] = L.r[j+1];
                L.r[j+1] = tmp;
                change = true;
            }
        }
    }
}

//5.希尔排序
void Shell(SqTable &L){
    for (int gap = L.len/2; gap > 0; gap /= 2){
        for (int i = gap + 1; i <= L.len; ++i){
            if (L.r[i] < L.r[i-gap]){
                ElemType tmp = L.r[i];
                int j = i - gap;
                while (j > 0 && L.r[j] > tmp){
                    L.r[j+gap] = L.r[j];
                    j -= gap;
                }
                L.r[j+gap] = tmp;
            }
        }
    }
}

// 主函数
int main() {
    int N = 0;
    cout<<"注意：N=10000的时候冒泡会很久，取值需谨慎\nN=?"<<endl;
    cin>>N;
    SqTable L;
    InitList(L, N);
    for (int i=1; i<=N; ++i) L.r[i] = i;
    RandomShuffleList(L);
    cout<<"\n随机数组生成成功"<<endl;
    clock_t begin, end;

    // =======================排序算法1：快速排序（Quick Sort）
    cout<<"\n快速排序计时中..."<<endl;
    SqTable L1;
    CopyList(L, L1);
    begin = clock(); // 计时器开始
    Quick(L1);
    end = clock(); // 计时器结束
    printf("快速排序用时: %g s\n", (float)(end-begin) / CLOCKS_PER_SEC);
    free(L1.r);
    cout<<"快速排序计时结束。"<<endl;

    // =======================排序算法2：堆排序（Heap Sort）
    cout<<"\n堆排序计时中..."<<endl;
    SqTable L2;
    CopyList(L, L2);
    begin = clock(); // 计时器开始
    Heap(L2);
    end = clock(); // 计时器结束
    printf("堆排序用时: %g s\n", (float)(end-begin) / CLOCKS_PER_SEC);
    free(L2.r);
    cout<<"堆排序计时结束。"<<endl;

    // =======================排序算法3：归并排序（Merge Sort）
    cout<<"\n归并排序计时中..."<<endl;
    SqTable L3;
    CopyList(L, L3);
    begin = clock(); // 计时器开始
    MergeSort(L3, 1, N);
    end = clock(); // 计时器结束
    printf("归并排序用时: %g s\n", (float)(end-begin) / CLOCKS_PER_SEC);
    free(L3.r);
    cout<<"归并排序计时结束。"<<endl;

    // =======================排序算法4：冒泡排序
    cout<<"\n冒泡排序计时中..."<<endl;
    SqTable L4;
    CopyList(L, L4);
    begin = clock(); // 计时器开始
    Bubble(L4);
    end = clock(); // 计时器结束
    printf("冒泡排序用时: %g s\n", (float)(end-begin) / CLOCKS_PER_SEC);
    free(L4.r);
    cout<<"冒泡排序计时结束。"<<endl;

    // =======================排序算法5：希尔排序
    cout<<"\n希尔排序计时中..."<<endl;
    SqTable L5;
    CopyList(L, L5);
    begin = clock(); // 计时器开始
    Shell(L5);
    end = clock(); // 计时器结束
    printf("希尔排序用时: %g s\n", (float)(end-begin) / CLOCKS_PER_SEC);
    free(L5.r);
    cout<<"希尔排序计时结束。"<<endl;

    free(L.r);
    return 0;
}