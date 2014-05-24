#include <stdio.h>
#include <stdlib.h>
int inversionPair;
int cct = 0;
void merge(int dst[], int lsc[], int lscSize, int rsc[], int rscSize);
void mergeSort(int arr[], int arrSize);
int main(int argc, char *argv[])
{
    int data[] = {0,2,1,3,5,4,9,10,6,7,8,11,20,21,29,22,23,12,13,30,14,15,16,17,18,19,24,25,28,31,26,27};
    int sortData[32];
    for (int i = 0; i < 32; i++) {
        sortData[i] = data[i];
    }
    inversionPair = 0;
    mergeSort(sortData, 32);
    for (int i = 0; i < 32; i++) {
        printf(" %d", sortData[i]);
    }
    printf("\ninversion: %d\ncct: %d\n", inversionPair, cct);
    return 0;
}

void mergeSort(int arr[], int arrSize) {
    if (arrSize <= 1) {
        // Do nothing
        return;
    }
    int lscSize = arrSize/2;
    int rscSize = arrSize - lscSize;
    int lsc[lscSize], rsc[rscSize];
    cct += lscSize+rscSize;
    for (int i = 0; i < lscSize; i++) {
        lsc[i] = arr[i];
    }
    for (int i = 0; i < rscSize; i++) {
        rsc[i] = arr[i+lscSize];
    }
    mergeSort(lsc, lscSize);
    mergeSort(rsc, rscSize);
    merge(arr, lsc, lscSize, rsc, rscSize);
}

void merge(int arr[], int lsc[], int lscSize, int rsc[], int rscSize) {
    int lct = 0, rct = 0, dstct = 0;
    while (lct < lscSize && rct < rscSize) {
        if (lsc[lct] >= rsc[rct]) {
            arr[dstct] = lsc[lct];
            dstct++;
            lct++;
        } else {
            arr[dstct] = rsc[rct];
            inversionPair += (lscSize-lct);
            dstct++;
            rct++;
        }
    }
    while (lct < lscSize) {
        arr[dstct] = lsc[lct];
        dstct++;
        lct++;
    }
    while (rct < rscSize) {
        arr[dstct] = rsc[rct];
        dstct++;
        rct++;
    }
}
