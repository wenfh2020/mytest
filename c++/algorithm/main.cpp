/* wenfahua - 2018-07-27 - sort code. */
#include <stdio.h>
#include <string.h>

void PrintArray(int szArray[], int iBegin, int iEnd) {
    for (int i = iBegin; i <= iEnd; i++) {
        printf("%.2d, ", szArray[i]);
    }
    printf("\n");
}

void QuickSort(int szArray[], int iBegin, int iEnd) {
    int iRight = iEnd;
    int iLeft = iBegin;
    int iMidValue = szArray[(iBegin + iEnd) / 2];

    while (iLeft <= iRight) {
        while (szArray[iLeft] < iMidValue && iLeft < iEnd) {
            iLeft++;
        }

        while (szArray[iRight] > iMidValue && iRight > iBegin) {
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

    printf("mid value = %.2d -- ", iMidValue);
    PrintArray(szArray, iBegin, iEnd);

    if (iBegin < iRight) {
        QuickSort(szArray, iBegin, iRight);
    }

    if (iLeft < iEnd) {
        QuickSort(szArray, iLeft, iEnd);
    }
}

int BinarySearch(int szArray[], int iSize, int iFindValue) {
    if (iSize <= 0) {
        return -1;
    }

    int iLeft = 0;
    int iRight = iSize - 1;

    while (iLeft <= iRight) {
        int iMid = ((unsigned int)(iLeft + iRight)) / 2; //《代码之美》说，left + right 有可能超出 int 是个负数~
        if (iFindValue > szArray[iMid]) {
            iLeft = iMid + 1;
        } else if (iFindValue < szArray[iMid]) {
            iRight = iMid - 1;
        } else {
            return iMid;
        }
    }

    return -1;
}

int main() {
    int szArray[11] = {10, 9, 8, 11, 6, 5, 10, 12, 1, 2, -1};
    PrintArray(szArray, 0, sizeof(szArray) / sizeof(int) - 1);
    QuickSort(szArray, 0, sizeof(szArray) / sizeof(int) - 1);
    PrintArray(szArray, 0, sizeof(szArray) / sizeof(int) - 1);

    int iFindValue = -1;
    printf("binary search(%d) result %d\n",
           iFindValue, BinarySearch(szArray, sizeof(szArray) / sizeof(int), iFindValue));
    return 0;
}
