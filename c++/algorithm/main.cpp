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
    int iLeft = 0;
    int iRight = iSize - 1;

    while (iLeft <= iRight) {
        int iMid = (iLeft + iRight) / 2;
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
    int szArray[10] = {10, 9, 8, 11, 6, 5, 10, 12, 1, 2};
    PrintArray(szArray, 0, sizeof(szArray) / sizeof(int) - 1);
    QuickSort(szArray, 0, sizeof(szArray) / sizeof(int) - 1);
    PrintArray(szArray, 0, sizeof(szArray) / sizeof(int) - 1);

    int iFindValue = 12;
    printf("binary search(%d) result %d",
           iFindValue, BinarySearch(szArray, sizeof(szArray) / sizeof(int), iFindValue));
    return 0;
}
