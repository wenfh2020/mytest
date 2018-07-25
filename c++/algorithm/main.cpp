#include <stdio.h>
#include <string.h>

//http://www.algolist.net/Algorithms/Sorting/Quicksort
//快速排序
void QuickSort(int szArray[], int iLeft, int iRight) {
    int iEnd = iRight;
    int iBegin = iLeft;
    int iFlag = szArray[(iLeft + iRight) / 2];

    /* partition */
    while (iBegin <= iEnd) {
        while (szArray[iBegin] < iFlag) {
            iBegin++;
        }

        while (szArray[iEnd] > iFlag) {
            iEnd--;
        }

        if (iBegin <= iEnd) {
            int iTemp = szArray[iBegin];
            szArray[iBegin] = szArray[iEnd];
            szArray[iEnd] = iTemp;
            iBegin++;
            iEnd--;
        }
    };

    /* recursion */
    if (iLeft < iEnd) {
        QuickSort(szArray, iLeft, iEnd);
    }

    if (iBegin < iRight) {
        QuickSort(szArray, iBegin, iRight);
    }
}

void PrintArray(int szArray[], int iLen) {
    for (int i = 0; i < iLen; i++) {
        printf("%d, ", szArray[i]);
    }
    printf("\n");
}

int main() {
    int szArray[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    size_t ulSize = sizeof(szArray) / sizeof(int);
    PrintArray(szArray, ulSize);

    QuickSort(szArray, 0, ulSize - 1);
    PrintArray(szArray, ulSize);
    return 0;
}
//二分法查找
