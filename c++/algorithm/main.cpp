/* wenfahua - 2018-07-27 - sort code. */
#include <stdio.h>
#include <string.h>

//www.algolist.net/Algorithms/Sorting/Quicksort
void QuickSort(int szArray[], int iBegin, int iEnd) {
    int iRight = iEnd;
    int iLeft = iBegin;
    int iFlag = szArray[(iBegin + iEnd) / 2];

    while (iLeft <= iRight) {
        while (szArray[iLeft] < iFlag && iLeft < iEnd) {
            iLeft++;
        }

        while (szArray[iRight] > iFlag && iRight > iBegin) {
            iRight--;
        }

        if (iLeft <= iRight) {
            int iTemp = szArray[iLeft];
            szArray[iLeft] = szArray[iRight];
            szArray[iRight] = iTemp;
            iLeft++;
            iRight--;
        }
    };

    if (iBegin < iRight) {
        QuickSort(szArray, iBegin, iRight);
    }

    if (iLeft < iEnd) {
        QuickSort(szArray, iLeft, iEnd);
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
