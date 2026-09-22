Write a C++ function named `insertIntoArray` that takes a fixed-size global integer array, its current logical size, an insertion index (1-based), and an integer item to insert. The function should insert the item at the specified index, shifting all subsequent elements to the right, and update the logical size. It must handle invalid indices: if the index is less than 1 (underflow) or greater than current logical size + 1 (overflow), it should do nothing and return the original logical size unchanged. The function should return the new logical size after a successful insertion, or the old size if invalid. The array has a maximum capacity of 30 elements, and you may assume the initial logical size is between 0 and 29. The function must be const-correct where appropriate and not rely on any global state other than the array itself being passed in.

The core algorithm involves shifting elements rightward starting from the last valid element down to the target index. Specifically, if the valid insertion position in 0-based terms is `index - 1`, we iterate `k` from the current logical size down to `index - 1`, copying `arr[k]` to `arr[k+1]`. This ensures the element at `index-1` becomes available, then we assign the new item there and increment the logical size. Edge cases: if index is out of bounds, return the original size without modifying the array. If the array is empty and index is 1, the loop does not execute and the item is placed at position 0. If inserting at the end (index = maxSize+1), the loop shifts nothing (since `k` starts at maxSize and runs while >= index-1, but index-1 = maxSize, so k = maxSize only, copying arr[maxSize] to arr[maxSize+1], which may be out of bounds if maxSize is 29 and array size is 30? Actually arr[maxSize] is valid until maxSize=29; for maxSize=29 and index=30, k=maxSize=29, we copy arr[29] to arr[30], but arr[30] is out of bounds for a 30-element array (indices 0-29). However, the problem states max capacity 30 elements, so logical size cannot exceed 29 before insertion, and after insertion max size is 30, which is okay if we allocate array size 30. But shifting from maxSize to maxSize+1 when maxSize=29 and array size 30: arr[30] is out of bounds. We need to be careful: if we insert at position maxSize+1, we just place item at arr[maxSize] without shifting. The standard shifting loop for insertion at any valid index should start from maxSize down to index-1, but if index-1 == maxSize, the loop condition `k >= maxSize` with k starting at maxSize will execute once, copying arr[maxSize] to arr[maxSize+1], which is out of bounds if array size is 30 and maxSize is 29. To avoid this, the loop should start from maxSize-1 down to index-1, then assign at index-1. Actually safer: shift elements from index-1 to maxSize-1 one position right, i.e., for k from maxSize-1 down to index-1, arr[k+1] = arr[k]. Then assign arr[index-1] = item. That works for all cases: if index = maxSize+1, then index-1 = maxSize, and the loop starts from maxSize-1 and goes down to maxSize, but condition k>=maxSize is false, so no shift, then assign arr[maxSize] = item. That is correct. So the correct approach: for (int k = maxSize - 1; k >= index - 1; --k) arr[k+1] = arr[k]. But careful with signed/unsigned. Since maxSize and index are ints, fine. Also assume the array has at least 30 elements. Time complexity O(n) where n is the logical size, due to shifting up to n elements. Space complexity O(1). Edge cases: index <= 0 underflow, index > maxSize+1 overflow, and also when array is full (maxSize == 30) we cannot insert, but given max size 30 and initial maxSize <= 29, we can insert at most once more. The function should also handle empty array.

#include <vector>
#include <cstddef>

// Insert item into arr at 1-based index, shifting right. Returns new logical size, or original if invalid.
int insertIntoArray(int arr[], int maxSize, int index, int item) {
    const int CAPACITY = 30;
    if (index < 1 || index > maxSize + 1 || maxSize >= CAPACITY) {
        return maxSize; // invalid index or full array
    }

    // Shift elements from the end down to the target position.
    for (int k = maxSize - 1; k >= index - 1; --k) {
        arr[k + 1] = arr[k];
    }

    arr[index - 1] = item;
    return maxSize + 1;
}

#include <cassert>
#include <iostream>

int main() {
    // Test 1: insert in the middle
    int arr1[30] = {1,2,3,4,5};
    int size1 = insertIntoArray(arr1, 5, 3, 99);
    assert(size1 == 6);
    assert(arr1[0]==1 && arr1[1]==2 && arr1[2]==99 && arr1[3]==3 && arr1[4]==4 && arr1[5]==5);

    // Test 2: insert at beginning
    int arr2[30] = {10,20,30};
    int size2 = insertIntoArray(arr2, 3, 1, 5);
    assert(size2 == 4);
    assert(arr2[0]==5 && arr2[1]==10 && arr2[2]==20 && arr2[3]==30);

    // Test 3: insert at end
    int arr3[30] = {7,8,9};
    int size3 = insertIntoArray(arr3, 3, 4, 100);
    assert(size3 == 4);
    assert(arr3[0]==7 && arr3[1]==8 && arr3[2]==9 && arr3[3]==100);

    // Test 4: invalid underflow index
    int arr4[30] = {1,2};
    int size4 = insertIntoArray(arr4, 2, 0, 42);
    assert(size4 == 2);
    assert(arr4[0]==1 && arr4[1]==2);

    // Test 5: invalid overflow index (beyond maxSize+1)
    int arr5[30] = {5,6};
    int size5 = insertIntoArray(arr5, 2, 4, 42);
    assert(size5 == 2);
    assert(arr5[0]==5 && arr5[1]==6);

    // Test 6: insert into empty array
    int arr6[30] = {};
    int size6 = insertIntoArray(arr6, 0, 1, 55);
    assert(size6 == 1);
    assert(arr6[0]==55);

    // Test 7: full array capacity (maxSize=29, insert at end)
    int arr7[30];
    for (int i = 0; i < 29; ++i) arr7[i] = i+1;
    int size7 = insertIntoArray(arr7, 29, 30, 999);
    assert(size7 == 30);
    assert(arr7[29] == 999);
    assert(arr7[0] == 1 && arr7[28] == 29);

    // Test 8: cannot insert when already full (maxSize=30)
    int arr8[30];
    for (int i = 0; i < 30; ++i) arr8[i] = i+1;
    int size8 = insertIntoArray(arr8, 30, 31, 123);
    assert(size8 == 30);

    return 0;
}
